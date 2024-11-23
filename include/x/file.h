#ifndef X_FILE_H
#define X_FILE_H

/**
 * @file file.h
 *
 * Download and pick files using the XViewFiles picker
 */

#include "o/file.h"


/**
 * Offers the given file as download
 * @param file to offer as download
 * @param try_system_first if true, o_file_download will be tested first
 * @param opt_event if not NULL, >MAY< be called on failure or success
 * @param event_user_data user data passed to the event
 * @note asserts the main thread
 */
O_EXTERN
void x_file_download(const char *file, bool try_system_first, o_file_event_fn opt_event, void *event_user_data);


/**
 * Creates a dialog for a file upload
 * @param file save destination, when uploaded
 * @param try_system_first if true, o_file_download will be tested first
 * @param opt_event if not NULL, >MAY< be called on failure or success
 * @param event_user_data user data passed to the event
 * @note asserts the main thread
 */
O_EXTERN
void x_file_upload(const char *file, bool try_system_first, o_file_event_fn opt_event, void *event_user_data);


#endif //X_FILE_H
