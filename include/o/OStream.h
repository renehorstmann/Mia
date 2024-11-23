#ifndef O_OSTREAM_H
#define O_OSTREAM_H

/**
 * @file OStream.h
 *
 * Object
 *
 * The stream obj is an abstract base object "interface" for io operations on files, sockets, memory, ...
 * The object "interface" matches the SDL_RWops interface (and is the most common sub object...)
 *
 * When subclassing, don't forget to set the destructor.
 *
 * @note OStreamBufferedWrite or smth is not implemented (needed...)
 */


#include "OObj.h"

/** object id */
#define OStream_ID OObj_ID "OStream"

/** Seek positions */
enum OStream_seek_whence {
    OStream_SEEK_SET,      // start of the stream
    OStream_SEEK_CUR,      // current position
    OStream_SEEK_END,      // end of the stream
    OStream_SEEK_NUM_WHENCES,
};

/**
 * Virtual is valid function.
 * @param obj OStream object
 * @return true if the stream is in a valid state (ready to stream...)
 */
typedef bool (*OStream__valid_fn)(oobj obj);

/**
 * Virtual size function.
 * @param obj OStream object
 * @return the size of the stream object, <0 for unknown or error
 */
typedef osize (*OStream__size_fn)(oobj obj);

/**
 * Virtual seek function.
 * @param obj OStream object
 * @param offset the byte offset to set (can be negative...)
 * @param whence specifies the relation to offset
 * @return the resulting offset from the beginning of the stream or <0 for errors | seek not supported
 */
typedef osize (*OStream__seek_fn)(oobj obj, osize offset, enum OStream_seek_whence whence);

/**
 * Virtual read function.
 * Tries to read up to num elements
 * @param obj OStream object
 * @param out_data Buffer to write the elements into
 * @param element_size size of a single element (sizeof(...))
 * @param up_to_num Numbers of elements to read
 * @return the number of elements read, which may be smaller than num!
 */
typedef osize (*OStream__read_try_fn)(oobj obj, void *out_data, osize element_size, osize up_to_num);

/**
 * Virtual write function.
 * @param obj OStream object
 * @param data Buffer to read the elements from
 * @param element_size size of a single element (sizeof(...))
 * @param num Numbers of elements to write
 * @return the number of elements written, or != num on failure
 */
typedef osize (*OStream__write_fn)(oobj obj, const void *data, osize element_size, osize num);

/**
 * Virtual closed function
 * @param OStream object
 * @return False if failed
 */
typedef bool (*OStream__close_fn)(oobj obj);


typedef struct {
    OObj super;

    // vfuncs
    OStream__valid_fn v_valid;
    OStream__size_fn v_size;
    OStream__seek_fn v_seek;
    OStream__read_try_fn v_read_try;
    OStream__write_fn v_write;
    OStream__close_fn v_close;
} OStream;

/**
 * Initializes the object, used by sub classes, so no new...
 * @param obj OStream object
 * @param parent to inherit from
 * @param valid_fn virtual valid function
 * @param size_fn virtual size function
 * @param seek_fn virtual seek function
 * @param read_try_fn virtual read_try function
 * @param write_fn virtual write function
 * @param close_fn virtual close function
 * @return obj casted as OStream
 */
O_EXTERN
OStream *OStream_init(oobj obj, oobj parent,
                      OStream__valid_fn valid_fn,
                      OStream__size_fn size_fn,
                      OStream__seek_fn seek_fn,
                      OStream__read_try_fn read_try_fn,
                      OStream__write_fn write_fn,
                      OStream__close_fn close_fn);

//
// virtual implementations:
//

/**
 * Default deletor that calls closed
 * Use this function as obj del in a sub class of OStream
 * @param obj OStream object
 */
O_EXTERN
void OStream__v_del(oobj obj);


//
// object functions:
//

/**
 * Calls the virtual is valid function.
 * @param obj OStream object, NULL safe
 * @return true if the stream is in a valid state (ready to stream...)
 */
O_INLINE
bool OStream_valid(oobj obj)
{
    if(!obj) {
        return false;
    }
    OObj_assert(obj, OStream);
    OStream *self = obj;
    return self->v_valid(self);
}


/**
 * Calls the virtual size function.
 * @param obj OStream object
 * @return the size of the stream object, <0 for unknown or error
 */
O_INLINE
osize OStream_size(oobj obj)
{
    OObj_assert(obj, OStream);
    OStream *self = obj;
    return self->v_size(self);
}


/**
 * Calls the virtual seek function.
 * @param obj OStream object
 * @param offset the byte offset to set
 * @param whence specifies the relation to offset
 * @return the resulting offset from the beginning of the stream or <0 for errors | seek not supported
 */
O_INLINE
osize OStream_seek(oobj obj, osize offset, enum OStream_seek_whence whence)
{
    OObj_assert(obj, OStream);
    OStream *self = obj;
    return self->v_seek(self, offset, whence);
}


/**
 * Calls the virtual read function.
 * Tries to read up to num elements
 * @param obj OStream object
 * @param out_data Buffer to write the elements into
 * @param element_size size of a single element (sizeof(...))
 * @param up_to_num Numbers of elements to read
 * @return the number of elements read, which may be smaller than num!
 */
O_INLINE
osize OStream_read_try(oobj obj, void *out_data, osize element_size, osize up_to_num)
{
    OObj_assert(obj, OStream);
    OStream *self = obj;
    return self->v_read_try(self, out_data, element_size, up_to_num);
}

/**
 * Calls the virtual write function.
 * @param obj OStream object
 * @param data Buffer to read the elements from
 * @param element_size size of a single element (sizeof(...))
 * @param num Numbers of elements to write
 * @return the number of elements written, or != num on failure
 */
O_INLINE
osize OStream_write(oobj obj, const void *data, osize element_size, osize num)
{
    OObj_assert(obj, OStream);
    OStream *self = obj;
    return self->v_write(self, data, element_size, num);
}


/**
 * Calls the virtual closed function
 * @param OStream object
 * @return False if failed
 */
O_INLINE
bool OStream_close(oobj obj)
{
    OObj_assert(obj, OStream);
    OStream *self = obj;
    return self->v_close(self);
}


/**
 * Reads exactly num*element_size bytes from the stream
 * @param obj OStream object
 * @param out_data Buffer to write the elements into
 * @param element_size size of a single element (sizeof(...))
 * @param num Numbers of elements to read
 * @return the number of elements read, or less in an error
 */
O_EXTERN
osize OStream_read(oobj obj, void *out_data, osize element_size, osize num);

/**
 * Writes the given string to the stream.
 * Macro, so the user does not always includes "o/str.h", but the compiler is able to inline
 * @param obj OStream object
 * @param text string to write
 * @return The number of written bytes
 * @note needs to include "o/str.h"
 */
#define OStream_print(obj, text) OStream_write((obj), (text), 1, o_strlen(text))

/**
 * Writes in a printf formatted style into the stream
 * va_list edition
 * @param obj OStream object
 * @param format printf style format
 * @param args var_args list
 * @return The number of written bytes
 */
O_EXTERN
osize OStream_printf_va(oobj obj, const char *format, va_list args);

/**
 * Writes in a printf formatted style into the stream
 * @param obj OStream object
 * @param format printf style format
 * @return The number of written bytes
 */
O_EXTERN
osize OStream_printf(oobj obj, const char *format, ...);

/**
 * Writes a line of text into the stream
 * @param obj OStream object
 * @param text string to append as line (null terminated...)
 * @return The number of written bytes
 */
O_INLINE
osize OStream_puts(oobj obj, const char *text)
{
    return OStream_printf(obj, "%s\n", text);
}

/**
 * Reads a single byte from the stream
 * @param obj OStream object
 * @return the character or -1 on error
 */
O_INLINE
int OStream_getchar(oobj obj)
{
    char c;
    if (OStream_read_try(obj, &c, 1, 1) <= 0) {
        return -1;
    }
    return (int) c;
}

/**
 * Reads from the stream into memory, until one of the given chars "until_character_set" (which is NOT included)
 * stops on max.
 * @param obj OStream object
 * @param out_opt_first_character_found the first character, thats in the set
 *                                      -1 on failure, NULL safe
 * @param out_opt_data data to write into (without the "until" character), or NULL
 * @param max out_data maximal buffer size | maximal chars to read (if out_opt_data, pass osize_MAX to ignore)
 * @param until_character_set multiple until characters in a null termninated string, ",." for eiter ',' or '.'
 * @return the characters read or -1 on error
 */
O_EXTERN
osize OStream_read_until_into(oobj obj,
                              int *out_opt_until_character_found,
                              char *out_opt_data, osize max,
                              const char *until_character_set);

/**
 * Reads from the stream into an allocated string,
 * until one of the given chars "until_character_set" (which will be included)
 * @param obj OStream object
 * @param out_opt_first_character_found the first character, thats in the set
 *                                      -1 on failure, NULL safe
 * @param max maximal chars to read (pass osize_MAX to ignore)
 * @param until character to read up to
 * @return the string read or NULL if failed
 */
O_EXTERN
char *OStream_read_until(oobj obj, int *out_opt_until_character_found,
                         osize max, const char *until_character_set);


/**
 * Reads from the stream into memory,
 *      until one of the given chars does not match the "all_character_set"
 * stops on max. If max is reached (return == max),
 * @param obj OStream object
 * @param out_opt_first_character_found the first character, thats not in the set
 *                                      -1 on failure, NULL safe
 * @param out_opt_data data to write into (without the "all" character), or NULL
 * @param max out_data maximal buffer size | maximal chars to read (if out_opt_data, pass osize_MAX to ignore)
 * @param all_character_set multiple characters in a null termninated string, ",." for eiter ',' or '.'
 * @return the characters read or -1 on error
 */
O_EXTERN
osize OStream_read_all_into(oobj obj, int *out_opt_first_character_found,
                            char *out_opt_data, osize max,
                            const char *all_character_set);

/**
 * Reads from the stream into memory,
 *      until one of the given chars does not match the "all_character_set"
 * @param obj OStream object
 * @param out_opt_first_character_found the first character, thats not in the set
 *                                  -1 on failure, NULL safe
 * @param max maximal chars to read (pass osize_MAX to ignore)
 * @param all_character_set multiple characters in a null termninated string, ",." for eiter ',' or '.'
 * @return the string read (without the "all" character), or NULL if failed
 */
O_EXTERN
char *OStream_read_all(oobj obj, int *out_opt_first_character_found, osize max, const char *all_character_set);





/**
 * Reads from the stream into memory, until the newline char '\n' (which will be included)
 * stops on max. If max is reached (return == max), check if the last character is the '\n' newline character...
 * @param obj OStream object
 * @param out_opt_data data to write into, or NULL
 * @param max out_data maximal buffer size | maximal chars to read (pass osize_MAX to ignore)
 * @return the characters read or -1 on error
 */
O_INLINE
osize OStream_read_line_into(oobj obj, char *out_opt_data, osize max)
{
    return OStream_read_until_into(obj, NULL, out_opt_data, max, "\n");
}

/**
 * Reads from the stream into an allocated string, until the newline char '\n' (which will be included)
 * @param obj OStream object
 * @param max maximal chars to read (pass osize_MAX to ignore)
 * @return the string read or NULL if failed
 */
O_INLINE
char *OStream_read_line(oobj obj, osize max)
{
    return OStream_read_until(obj, NULL, max, "\n");
}


/**
 * Reads from the stream into memory, as long as the character read is a "isspace"
 * stops on max. If max is reached (return == max)
 * @param obj OStream object
 * @param out_opt_first_character_found the first character, thats not in the set
 *                                  -1 on failure, NULL safe
 * @param out_opt_data data to write into, or NULL (does not the character from the "all" set)
 * @param max out_data maximal buffer size | maximal chars to read
 * @return the characters read or -1 on error
 */
O_INLINE
osize OStream_read_isspace_into(oobj obj, int *out_opt_first_character_found,
                                char *out_opt_data, osize max)
{
    return OStream_read_all_into(obj, out_opt_first_character_found, out_opt_data, max, O_ISSPACE_SET);
}

/**
 * Reads from the stream into memory, as long as the character read is a "isspace"
 * @param obj OStream object
 * @param out_opt_first_character_found the first character, thats not in the set
 *                                  -1 on failure, NULL safe
 * @param max maximal chars to read (pass osize_MAX to ignore)
 * @return the string read or NULL if failed
 */
O_INLINE
char *OStream_read_isspace(oobj obj, int *out_opt_first_character_found, osize max)
{
    return OStream_read_all(obj, out_opt_first_character_found, max, O_ISSPACE_SET);
}


#endif //O_OSTREAM_H
