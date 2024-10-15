#include "o/tar.h"
#include "o/OObjRoot.h"
#include "o/OStream.h"
#include "o/OArray.h"
#include "o/file.h"
#include "o/str.h"

#define O_LOG_LIB "o"
#include "o/log.h"



//
// clone of microtar: "https://github.com/rxi/microtar"
//

//
// microtar.h
//


#define MTAR_VERSION "0.1.0"

enum {
    MTAR_ESUCCESS = 0,
    MTAR_EFAILURE = -1,
    MTAR_EOPENFAIL = -2,
    MTAR_EREADFAIL = -3,
    MTAR_EWRITEFAIL = -4,
    MTAR_ESEEKFAIL = -5,
    MTAR_EBADCHKSUM = -6,
    MTAR_ENULLRECORD = -7,
    MTAR_ENOTFOUND = -8
};

enum {
    MTAR_TREG = '0',
    MTAR_TLNK = '1',
    MTAR_TSYM = '2',
    MTAR_TCHR = '3',
    MTAR_TBLK = '4',
    MTAR_TDIR = '5',
    MTAR_TFIFO = '6'
};

typedef struct {
    unsigned mode;
    unsigned owner;
    unsigned size;
    unsigned mtime;
    unsigned type;
    char name[100];
    char linkname[100];
} mtar_header_t;


typedef struct mtar_t mtar_t;

struct mtar_t {
    int (*read)(mtar_t *tar, void *data, unsigned size);

    int (*write)(mtar_t *tar, const void *data, unsigned size);

    int (*seek)(mtar_t *tar, unsigned pos);

    int (*close)(mtar_t *tar);

    void *stream;
    unsigned pos;
    unsigned remaining_data;
    unsigned last_header;
};

O_STATIC
const char *mtar_strerror(int err);

O_STATIC
//int mtar_open(mtar_t *tar, const char *filename, const char *mode);
int mtar_open(mtar_t *tar, oobj stream);

O_STATIC
int mtar_close(mtar_t *tar);

O_STATIC
int mtar_seek(mtar_t *tar, unsigned pos);

O_STATIC
int mtar_rewind(mtar_t *tar);

O_STATIC
int mtar_next(mtar_t *tar);

O_STATIC
int mtar_find(mtar_t *tar, const char *name, mtar_header_t *h);

O_STATIC
int mtar_read_header(mtar_t *tar, mtar_header_t *h);

O_STATIC
int mtar_read_data(mtar_t *tar, void *ptr, unsigned size);

O_STATIC
int mtar_write_header(mtar_t *tar, const mtar_header_t *h);

O_STATIC
int mtar_write_file_header(mtar_t *tar, const char *name, unsigned size);

O_STATIC
int mtar_write_dir_header(mtar_t *tar, const char *name);

O_STATIC
int mtar_write_data(mtar_t *tar, const void *data, unsigned size);

O_STATIC
int mtar_finalize(mtar_t *tar);


//
// microtar.c
//

typedef struct {
    char name[100];
    char mode[8];
    char owner[8];
    char group[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char type;
    char linkname[100];
    char _padding[255];
} mtar_raw_header_t;


O_STATIC
unsigned round_up(unsigned n, unsigned incr)
{
    return n + (incr - n % incr) % incr;
}


O_STATIC
unsigned checksum(const mtar_raw_header_t *rh)
{
    unsigned i;
    unsigned char *p = (unsigned char *) rh;
    unsigned res = 256;
    for (i = 0; i < offsetof(mtar_raw_header_t, checksum); i++) {
        res += p[i];
    }
    for (i = offsetof(mtar_raw_header_t, type); i < sizeof(*rh); i++) {
        res += p[i];
    }
    return res;
}


O_STATIC
int tread(mtar_t *tar, void *data, unsigned size)
{
    int err = tar->read(tar, data, size);
    tar->pos += size;
    return err;
}


O_STATIC
int twrite(mtar_t *tar, const void *data, unsigned size)
{
    int err = tar->write(tar, data, size);
    tar->pos += size;
    return err;
}


O_STATIC
int write_null_bytes(mtar_t *tar, int n)
{
    int i, err;
    char nul = '\0';
    for (i = 0; i < n; i++) {
        err = twrite(tar, &nul, 1);
        if (err) {
            return err;
        }
    }
    return MTAR_ESUCCESS;
}


O_STATIC
int raw_to_header(mtar_header_t *h, const mtar_raw_header_t *rh)
{
    unsigned chksum1, chksum2;

    /* If the checksum starts with a null byte we assume the record is NULL */
    if (*rh->checksum == '\0') {
        return MTAR_ENULLRECORD;
    }

    /* Build and compare checksum */
    chksum1 = checksum(rh);
    sscanf(rh->checksum, "%o", &chksum2);
    if (chksum1 != chksum2) {
        return MTAR_EBADCHKSUM;
    }

    /* Load raw header into header */
    sscanf(rh->mode, "%o", &h->mode);
    sscanf(rh->owner, "%o", &h->owner);
    sscanf(rh->size, "%o", &h->size);
    sscanf(rh->mtime, "%o", &h->mtime);
    h->type = rh->type;
    strcpy(h->name, rh->name);
    strcpy(h->linkname, rh->linkname);

    return MTAR_ESUCCESS;
}


O_STATIC
int header_to_raw(mtar_raw_header_t *rh, const mtar_header_t *h)
{
    unsigned chksum;

    /* Load header into raw header */
    memset(rh, 0, sizeof(*rh));
    sprintf(rh->mode, "%o", h->mode);
    sprintf(rh->owner, "%o", h->owner);
    sprintf(rh->size, "%o", h->size);
    sprintf(rh->mtime, "%o", h->mtime);
    rh->type = h->type ? h->type : MTAR_TREG;
    strcpy(rh->name, h->name);
    strcpy(rh->linkname, h->linkname);

    /* Calculate and write checksum */
    chksum = checksum(rh);
    sprintf(rh->checksum, "%06o", chksum);
    rh->checksum[7] = ' ';

    return MTAR_ESUCCESS;
}


const char *mtar_strerror(int err)
{
    switch (err) {
        case MTAR_ESUCCESS     :
            return "success";
        case MTAR_EFAILURE     :
            return "failure";
        case MTAR_EOPENFAIL    :
            return "could not open";
        case MTAR_EREADFAIL    :
            return "could not read";
        case MTAR_EWRITEFAIL   :
            return "could not write";
        case MTAR_ESEEKFAIL    :
            return "could not seek";
        case MTAR_EBADCHKSUM   :
            return "bad checksum";
        case MTAR_ENULLRECORD  :
            return "null record";
        case MTAR_ENOTFOUND    :
            return "name not found";
    }
    return "unknown error";
}


O_STATIC
int file_write(mtar_t *tar, const void *data, unsigned size)
{
//    unsigned res = fwrite(data, 1, size, tar->stream);
    osize res = OStream_write(tar->stream, data, 1, size);
    return (res == size) ? MTAR_ESUCCESS : MTAR_EWRITEFAIL;
}

O_STATIC
int file_read(mtar_t *tar, void *data, unsigned size)
{
//    unsigned res = fread(data, 1, size, tar->stream);
    osize res = OStream_read(tar->stream, data, 1, size);
    return (res == size) ? MTAR_ESUCCESS : MTAR_EREADFAIL;
}

O_STATIC
int file_seek(mtar_t *tar, unsigned offset)
{
//    int res = fseek(tar->stream, offset, SEEK_SET);
    osize res = OStream_seek(tar->stream, offset, SEEK_SET);
    return (res == offset) ? MTAR_ESUCCESS : MTAR_ESEEKFAIL;
}

O_STATIC
int file_close(mtar_t *tar)
{
//    fclose(tar->stream);
    return MTAR_ESUCCESS;
}


//int mtar_open(mtar_t *tar, const char *filename, const char *mode);
int mtar_open(mtar_t *tar, oobj stream)
{
    /* Init tar struct and functions */
    memset(tar, 0, sizeof(*tar));
    tar->write = file_write;
    tar->read = file_read;
    tar->seek = file_seek;
    tar->close = file_close;

//    /* Assure mode is always binary */
//    if ( strchr(mode, 'r') ) mode = "rb";
//    if ( strchr(mode, 'w') ) mode = "wb";
//    if ( strchr(mode, 'a') ) mode = "ab";
//    /* Open name */
//    tar->stream = fopen(filename, mode);
    tar->stream = stream;
    if (!tar->stream) {
        return MTAR_EOPENFAIL;
    }
//    /* Read first header to check it is valid if mode is `r` */
//    if (*mode == 'r') {
//        err = mtar_read_header(tar, &h);
//        if (err != MTAR_ESUCCESS) {
//            mtar_close(tar);
//            return err;
//        }
//    }

    /* Return ok */
    return MTAR_ESUCCESS;
}


int mtar_close(mtar_t *tar)
{
    return tar->close(tar);
}


int mtar_seek(mtar_t *tar, unsigned pos)
{
    int err = tar->seek(tar, pos);
    tar->pos = pos;
    return err;
}


int mtar_rewind(mtar_t *tar)
{
    tar->remaining_data = 0;
    tar->last_header = 0;
    return mtar_seek(tar, 0);
}


int mtar_next(mtar_t *tar)
{
    int err, n;
    mtar_header_t h;
    /* Load header */
    err = mtar_read_header(tar, &h);
    if (err) {
        return err;
    }
    /* Seek to next record */
    n = round_up(h.size, 512) + sizeof(mtar_raw_header_t);
    return mtar_seek(tar, tar->pos + n);
}


int mtar_find(mtar_t *tar, const char *name, mtar_header_t *h)
{
    int err;
    mtar_header_t header;
    /* Start at beginning */
    err = mtar_rewind(tar);
    if (err) {
        return err;
    }
    /* Iterate all files until we hit an error or find the name */
    while ((err = mtar_read_header(tar, &header)) == MTAR_ESUCCESS) {
        if (!strcmp(header.name, name)) {
            if (h) {
                *h = header;
            }
            return MTAR_ESUCCESS;
        }
        mtar_next(tar);
    }
    /* Return error */
    if (err == MTAR_ENULLRECORD) {
        err = MTAR_ENOTFOUND;
    }
    return err;
}


int mtar_read_header(mtar_t *tar, mtar_header_t *h)
{
    int err;
    mtar_raw_header_t rh;
    /* Save header position */
    tar->last_header = tar->pos;
    /* Read raw header */
    err = tread(tar, &rh, sizeof(rh));
    if (err) {
        return err;
    }
    /* Seek back to start of header */
    err = mtar_seek(tar, tar->last_header);
    if (err) {
        return err;
    }
    /* Load raw header into header struct and return */
    return raw_to_header(h, &rh);
}


int mtar_read_data(mtar_t *tar, void *ptr, unsigned size)
{
    int err;
    /* If we have no remaining data then this is the first read, we get the size,
     * set the remaining data and seek to the beginning of the data */
    if (tar->remaining_data == 0) {
        mtar_header_t h;
        /* Read header */
        err = mtar_read_header(tar, &h);
        if (err) {
            return err;
        }
        /* Seek past header and init remaining data */
        err = mtar_seek(tar, tar->pos + sizeof(mtar_raw_header_t));
        if (err) {
            return err;
        }
        tar->remaining_data = h.size;
    }
    /* Read data */
    err = tread(tar, ptr, size);
    if (err) {
        return err;
    }
    tar->remaining_data -= size;
    /* If there is no remaining data we've finished reading and seek back to the
     * header */
    if (tar->remaining_data == 0) {
        return mtar_seek(tar, tar->last_header);
    }
    return MTAR_ESUCCESS;
}


int mtar_write_header(mtar_t *tar, const mtar_header_t *h)
{
    mtar_raw_header_t rh;
    /* Build raw header and write */
    header_to_raw(&rh, h);
    tar->remaining_data = h->size;
    return twrite(tar, &rh, sizeof(rh));
}


int mtar_write_file_header(mtar_t *tar, const char *name, unsigned size)
{
    mtar_header_t h;
    /* Build header */
    memset(&h, 0, sizeof(h));
    strcpy(h.name, name);
    h.size = size;
    h.type = MTAR_TREG;
    h.mode = 0664;
    /* Write header */
    return mtar_write_header(tar, &h);
}


int mtar_write_dir_header(mtar_t *tar, const char *name)
{
    mtar_header_t h;
    /* Build header */
    memset(&h, 0, sizeof(h));
    strcpy(h.name, name);
    h.type = MTAR_TDIR;
    h.mode = 0775;
    /* Write header */
    return mtar_write_header(tar, &h);
}


int mtar_write_data(mtar_t *tar, const void *data, unsigned size)
{
    int err;
    /* Write data */
    err = twrite(tar, data, size);
    if (err) {
        return err;
    }
    tar->remaining_data -= size;
    /* Write padding if we've written all the data for this name */
    if (tar->remaining_data == 0) {
        return write_null_bytes(tar, round_up(tar->pos, 512) - tar->pos);
    }
    return MTAR_ESUCCESS;
}


int mtar_finalize(mtar_t *tar)
{
    /* Write two NULL records */
    return write_null_bytes(tar, sizeof(mtar_raw_header_t) * 2);
}

//
// public
//

osize o_tar_read_stream(oobj obj, struct o_tar_file **out_files, oobj stream)
{
    OObj_assert(stream, OStream);

    *out_files = NULL;
    osize files_num = 0;

    mtar_t tar;
    if(mtar_open(&tar, stream) != MTAR_ESUCCESS) {
        o_log_wtf_s(__func__, "failed to open?");
        return 0;
    }

    // collect name info
    oobj files = OArray_new_dyn(obj, NULL, sizeof (struct o_tar_file), 0, 16);


    /* Print all name names and sizes */
    mtar_header_t h;
    while ((mtar_read_header(&tar, &h)) != MTAR_ENULLRECORD) {
        struct o_tar_file f = {0};
        o_strf_buf(f.name, "%s", h.name);
        f.data_size = h.size;
        OArray_push(files, &f);

        if(mtar_next(&tar) != MTAR_ESUCCESS) {
            o_log_error_s(__func__, "failed to get next header element");
            goto CLEAN_UP;
        }
    }

    for(osize i=0; i < OArray_num(files); i++) {
        struct o_tar_file *f = OArray_at(files, i, struct o_tar_file);
        // also null terminated
        f->data = o_alloc0(obj, 1, f->data_size+1);
        if(mtar_find(&tar, f->name, &h) != MTAR_ESUCCESS) {
            o_log_error_s(__func__, "failed to find: \"%s\"", f->name);
            goto CLEAN_UP;
        }
        if(mtar_read_data(&tar, f->data, f->data_size) != MTAR_ESUCCESS) {
            o_log_error_s(__func__,
                       "failed to read data for: \"%s\" (%i bytes)", f->name, (int) f->data_size);
            goto CLEAN_UP;
        }
    }

    // success, move data
    files_num = OArray_num(files);
    *out_files = OArray_move(files, obj);


    CLEAN_UP:
    for(osize i=0; i < OArray_num(files); i++) {
        struct o_tar_file *f = OArray_at(files, i, struct o_tar_file);
        o_free(obj, f->data);
    }
    o_del(files);

    /* Close archive */
    mtar_close(&tar);

    return files_num;
}

osize o_tar_read_file(oobj obj, struct o_tar_file **out_files, const char *filename)
{
    struct oobj_opt stream = o_file_open(obj, filename, "rb");
    if(!OStream_valid(stream.o)) {
        o_log_warn_s(__func__,
                   "failed to open the file: %s", filename);
        return 0;
    }
    osize cnt = o_tar_read_stream(obj, out_files, stream.o);
    o_del(stream.o);
    return cnt;
}


bool o_tar_write_stream(oobj stream, const struct o_tar_file *files, osize files_size)
{
    OObj_assert(stream, OStream);

    bool ok = true;

    mtar_t tar;
    if(mtar_open(&tar, stream) != MTAR_ESUCCESS) {
        o_log_wtf_s(__func__, "failed to open?");
        return false;
    }

    for(osize i=0; i<files_size; i++) {
        if(files[i].name[0] == '\0' || !files[i].data || files[i].data_size <= 0) {
            o_log_warn_s(__func__, "invalid name, ignoring");
            continue;
        }

        o_log("writing: %s %i | %i", files[i].name, (int) files[i].data_size, (int) strlen(files[i].data));

        if(mtar_write_file_header(&tar, files[i].name, files[i].data_size) != MTAR_ESUCCESS) {
            o_log_error_s(__func__, "failed to write header: \"%s\"", files[i].name);
            goto CLEAN_UP;
        }


        if(mtar_write_data(&tar, files[i].data, files[i].data_size) != MTAR_ESUCCESS) {
            o_log_error_s(__func__,
                       "failed to write data: \"%s\" (%i bytes)", files[i].name, files[i].data_size);
            goto CLEAN_UP;
        }
    }


    CLEAN_UP:

    /* Finalize -- this needs to be the last thing done before closing */
    mtar_finalize(&tar);

    /* Close archive */
    mtar_close(&tar);

    return ok;
}

bool o_tar_write_file(const char *filename, const struct o_tar_file *files, osize files_size)
{
    oobj root = OObjRoot_new_heap();
    struct oobj_opt stream = o_file_open(root, filename, "wb");
    bool ok = false;
    if(!OStream_valid(stream.o)) {
        o_log_warn_s(__func__,
                   "failed to open the file: %s", filename);
    } else {
        ok = o_tar_write_stream(stream.o, files, files_size);
    }
    o_del(root);
    return ok;
}
