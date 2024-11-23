#ifndef O_IMAGE_H
#define O_IMAGE_H

/**
 * @file log.h
 *
 * Load and write images in rgba8 format (png and jpg).
 * Based on SDL_image
 */

#include "o/common.h"

struct o_img {
    // row major order, beginning at bottom left
    obyte *rgba_data;
    int cols, rows;
    
    // may be NULL
    oobj parent;
};

/**
 * Creates empty memory for an image
 * @param obj to allocate the resource
 * @param cols columns to allocate
 * @param rows to allocate
 * @return a new o_img struct, .rgba_data owned by obj, or {0} if failed
 * @note UNLIKE MOST IMAGE LOADERS, DATA BEGINS AT BOTTOM LEFT, instead top left
 */
O_EXTERN
struct o_img o_img_new(oobj obj, int cols, int rows);

/**
 * Creates cleared to zero memory for an image
 * @param obj to allocate the resource
 * @param cols columns to allocate
 * @param rows to allocate
 * @return a new o_img struct, .rgba_data owned by obj, or {0} if failed
 * @note UNLIKE MOST IMAGE LOADERS, DATA BEGINS AT BOTTOM LEFT, instead top left
 */
O_EXTERN
struct o_img o_img_new0(oobj obj, int cols, int rows);

/**
 * Loads an image file as rgba 8bit into memory.
 * The first pixel is at bottom left in row_major.
 * @param obj to allocate the resource
 * @param file image file (png, jpg)
 * @return a new o_img struct, .rgba_data owned by obj, or {0} if failed
 * @note UNLIKE MOST IMAGE LOADERS, DATA BEGINS AT BOTTOM LEFT, instead top left
 */
O_EXTERN
struct o_img o_img_new_file(oobj obj, const char *file);

/**
 * moves the image rgba_data into another object
 * @param self to be moved
 * @param into the new object to move the list into
 * @note just calls o_move_res(obj, into, self.rgba_data)
 */
O_EXTERN
void o_img_move(struct o_img *self, oobj into);

/**
 * frees the image rgba_data
 * @param self to free
 * @note just calls o_free(obj, self.rgba_data)
 */
O_EXTERN
void o_img_free(struct o_img *self);

/**
 * Creates an SDL surface from he image
 * @param self to create the surface from
 * @note call SDL_FreeSurface on the returned surface
 */
O_EXTERN
void *o_img_as_sdl_surface(struct o_img self);

//
// struct object functions
//

/**
 * @return cols * rows
 */
O_INLINE
osize o_img_num(struct o_img self)
{
    return (osize) self.cols * (osize) self.rows;
}

/**
 * @return size in bytes needed to hold the image data
 */
O_INLINE
osize o_img_data_size(struct o_img self)
{
    return o_img_num(self) * 4;
}



/**
 * Writes an image into a png file
 * @param self the image to write (rgba 8 bit)
 * @param file to write into
 * @return false if failed
 * @note UNLIKE MOST IMAGE LOADERS, DATA BEGINS AT BOTTOM LEFT, instead top left
 */
O_EXTERN
bool o_img_write_file(struct o_img self, const char *file);

/**
 * @param out uninitialized data, but must have been allocated before
 * @param src to be flipped into out
 * @note asserts out sizes match src sizes
 */
O_EXTERN
void o_img_flip_rows(struct o_img out, struct o_img src);

/**
 * @param self the image to get a pixel from
 * @param idx of a pixel
 * @return the pointer to the pixel at the given pos (starting at r (gba))
 * @note asserts in bounds
 *       UNLIKE MOST IMAGE LOADERS, DATA BEGINS AT BOTTOM LEFT, instead top left
 */
O_INLINE
obyte *o_img_at_idx(struct o_img self, int idx)
{
    assert(self.rgba_data && 0<=idx && idx<self.cols * self.rows && "idx out of o_img bounds");
    return &self.rgba_data[idx*4];
}

/**
 * @param self the image to get a pixel from
 * @param c the column, 0 is left
 * @param r the row, 0 is bottom
 * @return the pointer to the pixel at the given pos (starting at r (gba))
 * @note asserts in bounds
 *       UNLIKE MOST IMAGE LOADERS, DATA BEGINS AT BOTTOM LEFT, instead top left
 */
O_INLINE
obyte *o_img_at(struct o_img self, int c, int r)
{
    assert(self.rgba_data && 0<=c && c<self.cols && 0<=r && r < self.rows && "idx out of o_img bounds");
    return o_img_at_idx(self, r * self.cols + c);
}

#endif //O_IMAGE_H
