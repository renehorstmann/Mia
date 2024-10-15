#ifndef O_FILE_H
#define O_FILE_H

/**
 * @file file.h
 *
 * Open files as streams or into memory.
 *
 * Functions to handle record files (for web / emscripten).
 */


#include "OStream.h"
#include "OArray.h"

/** excluding the null terminator */
#define O_FILE_RECORD_MAX_FILE_LENGTH 128

/** maximal size of a path, including the null terminator */
#define O_FILE_PATH_MAX 4096

/**
 * Modes for o_file_list.
 * FILES is for regular files
 */
enum o_file_list_mode {
    o_file_list_ALL,
    o_file_list_DIRS,
    o_file_list_FILES,
    o_file_list_NUM_MODES
};

/**
 * Differnet home locations.
 * Default results in different location based on the platform (android -> INTERNAL)
 */
enum o_file_home_loc {
    o_file_home_DEFAULT,
    o_file_home_WORKING,
    o_file_home_INTERNAL,
    o_file_home_EXTERNAL,
    o_file_home_NUM_LOCS
};

/**
 * Optional finished event function, >MAY< be called.
 * @param file is the given file
 * @param opt_user_file may be NULL or the user selected file
 * @param success true if cloned some bytes
 */
typedef void (*o_file_event_fn)(void *user_data, const char *file, const char *opt_user_file, bool success);


/**
 * Opens a file as OStream.
 *
 * The text below is a copy of SDL
 *
 * The `mode` string is treated roughly the same as in a call to the C
 * library's fopen(), even if SDL doesn't happen to use fopen() behind the
 * scenes.
 *
 * Available `mode` strings:
 *
 * - "r": Open a file for reading. The file must exist.
 * - "w": Create an empty file for writing. If a file with the same name
 *   already exists its content is erased and the name is treated as a new
 *   empty file.
 * - "a": Append to a file. Writing operations append data at the end of the
 *   file. The file is created if it does not exist.
 * - "r+": Open a file for update both reading and writing. The file must
 *   exist.
 * - "w+": Create an empty file for both reading and writing. If a file with
 *   the same name already exists its content is erased and the file is
 *   treated as a new empty file.
 * - "a+": Open a file for reading and appending. All writing operations are
 *   performed at the end of the file, protecting the previous content to be
 *   overwritten. You can reposition (fseek, rewind) the internal pointer to
 *   anywhere in the file for reading, but writing operations will move it
 *   back to the end of file. The file is created if it does not exist.
 *
 * **NOTE**: In order to open a file as a binary file, a "b" character has to
 * be included in the `mode` string. This additional "b" character can either
 * be appended at the end of the string (thus making the following compound
 * modes: "rb", "wb", "ab", "r+b", "w+b", "a+b") or be inserted between the
 * letter and the "+" sign for the mixed modes ("rb+", "wb+", "ab+").
 * Additional characters may follow the sequence, although they should have no
 * effect. For example, "t" is sometimes appended to make explicit the file is
 * a text file.
 *
 * This function supports Unicode filenames, but they must be encoded in UTF-8
 * format, regardless of the underlying operating system.
 *
 * @param parent OStream will be a resource of parent.
 * @param file a UTF-8 string representing the filename to open.
 * @param mode an ASCII string representing the mode to be used for opening.
 *             the file.
 * @return an OStream object to read and write with the file. NULL on error
 */
O_EXTERN
struct oobj_opt o_file_open(oobj parent, const char *file, const char *mode);


/**
 * Creates an OStream object from the given C FILE
 * @param parent OStream will be a resource of parent.
 * @param file The C FILE, closed if auto_close is set to true
 * @param auto_close if true, the file will get closed by the stream
 * @return an OStream object to read and write with the file. NULL on error
 * @note can be used with stdin, stdout, stderr
 */
O_EXTERN
struct oobj_opt o_file_c(oobj parent, FILE *file, bool auto_close);

/**
 * Reads in the full given file.
 * @param parent OArray will be a resource of parent.
 * @param file filename to open.
 * @param ascii If true, the file will be read in ascii mode, else binary mode
 * @param element_size For the OArray, (sizeof(...))
 * @return A new OArray object, containing the full file, or NULL on error
 */
O_EXTERN
struct oobj_opt o_file_read(oobj parent, const char *file, bool ascii, osize element_size);


/**
 * Writes into the given file
 * @param parent Used for internal allocations
 * @param file filename to save in
 * @param ascii If true, the file will be written in ascii mode, else binary mode
 * @param data data to write
 * @param element_size sizeof(...)
 * @param num of elements to write
 * @return number of written elements (!=num on failure)
 */
O_EXTERN
osize o_file_write(const char *file, bool ascii, const void *data, osize element_size, osize num);



/**
 * Appends to the given file
 * @param parent Used for internal allocations
 * @param file filename to save in
 * @param ascii If true, the file will be written in ascii mode, else binary mode
 * @param data data to write
 * @param element_size sizeof(...)
 * @param num of elements to write
 * @return number of written elements (!=num on failure)
 */
O_EXTERN
osize o_file_append(const char *file, bool ascii, const void *data, osize element_size, osize num);

/**
 * Clones file src into file dst
 * @param dst file to be cloned into
 * @param src file to be cloned from
 * @return bytes written | cloned
 */
O_EXTERN
osize o_file_clone(const char *dst, const char *src);


/**
 * @param file path
 * @return true if that file is a directory
 */
O_EXTERN
bool o_file_is_dir(const char *file);

/**
 * @param file path
 * @return true if that file is a regular file
 */
O_EXTERN
bool o_file_is_regular(const char *file);

/**
 * @param path directory path to create
 * @return true if path was created successfully or already existed
 */
O_EXTERN
bool o_file_mkdirs(const char *path);


/**
 * @param location one of the different home locations the app may have
 * @return a static const null terminated string to the desired home
 */
O_EXTERN
const char *o_file_home(enum o_file_home_loc location);

/**
 * @param parent to allocate on
 * @param directory to list the files in, NULL safe (-> ".")
 * @param mode different modes to only list dirs or files, etc.
 * @param file_filter a null terminated list of strings of file endings (case is ignored)
 * @return OArray of file strings (char *) (empty on access error)
 * @note MIA_PLATFORM_EMSCRIPTEN returns an empty list
 */
O_EXTERN
oobj o_file_list(oobj parent, const char *directory, enum o_file_list_mode mode, char **file_filter);

/**
 * Returns a valid record name for the given filename.
 * On MIA_PLATFORM_EMSCRIPTEN:    "record/filename
 * Else:                        "record.filename
 * @param out_record_name A buffer of at least O_FILE_PATH_MAX size, as result record name
 * @param filename must NOT include directories '/'
 * @note asserts a valid filename (no directories '/', not NULL)
 * @threadsafe
 */
O_EXTERN
void o_file_record_name_into(char *out_record_name, const char *filename);


/**
 * Returns a valid record name for the given filename.
 * Valid until the calling thread recalls this function.
 * On MIA_PLATFORM_EMSCRIPTEN:    "record/filename
 * Else:                        "record.filename
 * @param filename must NOT include directories '/'
 * @return a threadlocal name for that filename
 * @note asserts a valid filename (no directories '/', not NULL)
 * @threadsafe uses threadlocal storage
 */
O_EXTERN
char *o_file_record_name(const char *filename);

/**
 * Syncs the record files.
 * Should be called after writing / appending a record file so sync and save.
 * On MIA_PLATFORM_EMSCRIPTEN:  syncs the indexedDB
 * Else:                        NoOp
 * @note no need to be threadsafe, emscripten is single thread in this framework
 */
O_EXTERN
void o_file_record_sync(void);


/**
 * Offers the given file as download via the system dialog
 * @param file to offer as download
 * @param opt_event if not NULL, >MAY< be called on failure or success
 * @param event_user_data user data passed to the event
 * @note asserts the main thread
 */
O_EXTERN
void o_file_download(const char *file, o_file_event_fn opt_event, void *event_user_data);


/**
 * Creates a system dialog for a file upload
 * @param file save destination, when uploaded
 * @param opt_event if not NULL, >MAY< be called on failure or success
 * @param event_user_data user data passed to the event
 * @note asserts the main thread
 */
O_EXTERN
void o_file_upload(const char *file, o_file_event_fn opt_event, void *event_user_data);


#endif //O_FILE_H
