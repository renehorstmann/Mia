#ifndef O_TAR_H
#define O_TAR_H

/**
 * @file tar.h
 *
 * Save and load .tar archives with streams.
 * A .tar is just a collection of uncompressed files "tape archive".
 *
 * Makes use of microtar: "https://github.com/rxi/microtar"
 */

#include "common.h"

// tar maximal file name is limited to 100 characters (I don't know if w/ or w/o the null terminator)
#define O_TAR_FILE_MAX 99
#define O_TAR_FILE_MAX_BUFFER (O_TAR_FILE_MAX + 1)


/**
 * .tar file to read or write
 */
struct o_tar_file {
    // file name for the archive, may include directories with '/'
    char name[O_TAR_FILE_MAX_BUFFER];
    void *data;
    osize data_size;
};


/**
 * Reads in files from a .tar by stream
 * @param obj To allocate the result on
 * @param out_files result list of files, cleared element terminated, or use the return size of files
 *                  each .data buffer is also null terminated
 * @param stream OStream object to read from
 * @return the number of files read in that tar, or 0 if failed
 * @note stream must be fully seekable! (name.h, OStreamArray)
 */
O_EXTERN
osize o_tar_read_stream(oobj obj, struct o_tar_file **out_files, oobj stream);

/**
 * Reads in files from a .tar name
 * @param obj To allocate the result on
 * @param out_files result list of files, cleared element terminated, or use the return size of files
 *                  each .data buffer is also null terminated
 * @param filename name path to load and read
 * @return the number of files read in that tar, or 0 if failed
 */
O_EXTERN
osize o_tar_read_file(oobj obj, struct o_tar_file **out_files, const char *filename);


/**
 * Writes the given files into the .tar stream
 * @param stream to write the .tar archive into
 * @param files list of files to write
 * @param files_size list size | number of files to write into the ,tar archive
 * @return false on error
 * @note stream must be fully seekable! (name.h, OStreamArray)
 */
O_EXTERN
bool o_tar_write_stream(oobj stream, const struct o_tar_file *files, osize files_size);

/**
 * Writes the given files into the .tar name
 * @param filename to write the .tar archive into
 * @param files list of files to write
 * @param files_size list size | number of files to write into the ,tar archive
 * @return false on error
 */
O_EXTERN
bool o_tar_write_file(const char *filename, const struct o_tar_file *files, osize files_size);

#endif //O_TAR_H
