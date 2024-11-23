#ifndef U_UIMAGE_H
#define U_UIMAGE_H

/**
 * @file UImg.h
 *
 * Object
 *
 * An image to load, save and manipulate.
 *
 * Operators:
 * o_num -> UImg_num
 * o_at -> UImg_at_idx
 */


#include "o/OObj.h"
#include "o/OTask.h"
#include "r/RTex.h"
#include "m/types/byte.h"


/** object id */
#define UImg_ID OObj_ID "UImg"


/**
 * start modes for UImg_collage (Left, Right, Bottom, Top)
 */
enum UImg_start
{
    UImg_START_LB,
    UImg_START_RB,
    UImg_START_LT,
    UImg_START_RT,
    UImg_START_NUM_MODES
};


typedef struct
{
    OObj super;

    obyte* restrict data;
    enum r_format format;

    // const, create a new img to change
    ivec2 size;
} UImg;


/**
 * Initializes the object
 * @param obj UImg object
 * @param parent to inherit from
 * @param opt_data if not NULL, copied from, else untouched empty allocation
 * @param cols size of the image
 * @param rows size of the image
 * @param format used format for the image and opt_data to be copied from
 * @return obj casted as UImg
 */
O_EXTERN
UImg* UImg_init(oobj obj, oobj parent, const void* opt_data, int cols, int rows, enum r_format format);

/**
 * Creates a new the UImg object
 * @param parent to inherit from
 * @param opt_data if not NULL, copied from, else untouched empty allocation
 * @param cols size of the image
 * @param rows size of the image
 * @param format used format for the image and opt_data to be copied from
 * @return The new object
 */
O_INLINE
UImg* UImg_new(oobj parent, const void* opt_data, int cols, int rows, enum r_format format)
{
    OObj_DECL_IMPL_NEW(UImg, parent, opt_data, cols, rows, format);
}


/**
 * Creates a new the UImg object.
 * All pixels are cleared to 0.
 * @param parent to inherit from
 * @param cols size of the image
 * @param rows size of the image
 * @param format used format for the image
 * @return The new object
 */
O_EXTERN
UImg* UImg_new_0(oobj parent, int cols, int rows, enum r_format format);


/**
 * Creates a new the UImg object.
 * @param parent to inherit from
 * @param file to load the image from, either .jpg or .png
 * @return The new object, or NULL if failed
 * @note uses R_FORMAT_RGBA_8
 * @warning MAY RETURN NULL IF FAILED
 */
O_EXTERN
struct oobj_opt UImg_new_file(oobj parent, const char* file);

/**
 * Creates a new the UImg object.
 * @param parent to inherit from
 * @param tex RTex to load the image from
 * @param format used format for the image
 * @return The new object, or NULL if failed (tex is NULL)
 * @warning MAY RETURN NULL IF FAILED
 */
O_EXTERN
struct oobj_opt UImg_new_tex(oobj parent, oobj tex, enum r_format format);

//
// virtual implementations:
//

/**
 * virtual operator function
 * @param obj UImg object
 * @return number of pixels (cols * rows)
 */
O_EXTERN
osize UImg__v_op_num(oobj obj);

/**
 * virtual operator function
 * @param obj UImg object
 * @param pixel data at given index
 */
O_EXTERN
void* UImg__v_op_at(oobj obj, osize idx);

//
// object functions
//


/**
 * @param obj UImg object
 * @return data pixels of the image
 */
OObj_DECL_GET(UImg, void *, data)

/**
 * @param obj UImg object
 * @return current pixel format
 */
OObj_DECL_GET(UImg, enum r_format, format)


/**
 * @param obj UImg object
 * @return number of pixels (cols * rows)
 */
O_INLINE
osize UImg_num(oobj obj)
{
    OObj_assert(obj, UImg);
    UImg* self = obj;
    return (osize)self->size.x * (osize)self->size.y;
}

/**
 * @param obj UImg
 * @return size in bytes needed to hold the image data
 */
O_INLINE
osize UImg_data_size(oobj obj)
{
    OObj_assert(obj, UImg);
    UImg* self = obj;
    return UImg_num(obj) * r_format_size(self->format);
}

/**
 * @param obj UImg
 * @param idx of a pixel
 * @return the pointer to the pixel at the given pos (starting at r (gba))
 * @note asserts in bounds
 *       UNLIKE MOST IMAGE LOADERS, DATA BEGINS AT BOTTOM LEFT, instead top left
 */
O_INLINE
void* UImg_at_idx(oobj obj, osize idx)
{
    OObj_assert(obj, UImg);
    UImg* self = obj;
    assert(self->data && 0<=idx && idx<UImg_num(self) && "idx out of o_img bounds");
    return &self->data[idx * r_format_size(self->format)];
}

/**
 * @param obj UImg
 * @param c the column, 0 is left
 * @param r the row, 0 is bottom
 * @return the pointer to the pixel at the given pos (starting at r (gba))
 * @note asserts in bounds
 *       UNLIKE MOST IMAGE LOADERS, DATA BEGINS AT BOTTOM LEFT, instead top left
 */
O_INLINE
void* UImg_at(oobj obj, int c, int r)
{
    OObj_assert(obj, UImg);
    UImg* self = obj;
    assert(self->data && 0<=c && c<self->size.x && 0<=r && r < self->size.y && "idx out of o_img bounds");
    return UImg_at_idx(self, r * self->size.x + c);
}

/**
 * @param obj UImg object
 * @return size of the allocated image
 */
O_INLINE
ivec2 UImg_size_int(oobj obj)
{
    OObj_assert(obj, UImg);
    UImg* self = obj;
    return self->size;
}

/**
 * @param obj UImg object
 * @returnsize of the allocated image
 * @note integer values
 */
O_INLINE
vec2 UImg_size(oobj obj)
{
    ivec2 size = UImg_size_int(obj);
    return vec2_(size.x, size.y);
}

/**
 * @param obj UImg object
 * @param pos centered
 * @param size of the sprite
 * @return UImg left bottom to sprite left bottom
 */
O_INLINE
vec2 UImg_pos_to_offset_lb(oobj obj, vec2 pos, vec2 size)
{
    OObj_assert(obj, UImg);
    UImg* self = obj;
    return vec2_(
        pos.x - size.x/2.0f + self->size.x / 2.0f,
        pos.y - size.y/2.0f + self->size.y / 2.0f
    );
}

/**
 * @param obj UImg object
 * @return RTex from this image
 */
O_EXTERN
RTex* UImg_tex(oobj obj);

/**
 * Writes the image to the given file as png.
 * @param obj UImg object
 * @parem file to save the image into
 * @return false if failed
 * @note asserts format=R_FORMAT_RGBA_8
 */
O_EXTERN
bool UImg_write_file(oobj obj, const char* file);


/**
 * sets the whole image to clear_color
 * @param obj UImg object
 * @param clear_color color to set
 */
O_EXTERN
void UImg_clear(oobj obj, vec4 clear_color);

/**
 * @param obj UImg object
 * @return smallest values for each channel [0:1] (ou8 is casted)
 */
O_EXTERN
vec4 UImg_min(oobj obj);

/**
 * @param obj UImg object
 * @return highest values for each channel [0:1] (ou8 is casted)
 */
O_EXTERN
vec4 UImg_max(oobj obj);

/**
 * Scales one or more channels in this image
 * @param obj UImg object
 * @param add for each channel, e. g. to add only alpha use vec4_(0, 0, 0, X)
 * @note resulting pixel values are clamped [0:1] or [0:255].
 *       Every pixel is casted into a vec4, before adding, so for ou8 type, use X/255.0f for adding
 */
O_EXTERN
void UImg_add(oobj obj, vec4 add);

/**
 * Scales one or more channels in this image
 * @param obj UImg object
 * @param scale for each channel, e. g. to scale only alpha use vec4_(1, 1, 1, X)
 * @note resulting pixel values are clamped [0:1] or [0:255].
 */
O_EXTERN
void UImg_scale(oobj obj, vec4 scale);

/**
 * Scales one or more channels in this image.
 * result = (obj + add) * scale
 * @param obj UImg object
 * @param add for each channel, e. g. to add only alpha use vec4_(0, 0, 0, X)
 * @param scale for each channel, e. g. to scale only alpha use vec4_(1, 1, 1, X)
 * @note resulting pixel values are clamped [0:1] or [0:255]. #
 *       Every pixel is casted into a vec4, before adding, so for ou8 type, use X/255.0f for adding

 */
O_EXTERN
void UImg_add_scaled(oobj obj, vec4 add, vec4 scale);

/**
 * Normalizes the image, so that the previous min value gets 0 and max -> 1
 * @param obj
 */
O_EXTERN
void UImg_normalize(oobj obj);

/**
 * Blits (copies) the UImg img into obj, with the given offset from obj:lb to img:lb, ignores out of bounds
 * @param obj UImg object
 * @param img UImg object to copy onto obj
 * @param offset_lb from left, bottom
 * @note just copies the values, no blending internally
 *       see UImg_blit for the centered version
 */
O_EXTERN
void UImg_blit_lb(oobj obj, oobj img, ivec2 offset_lb);

/**
 * Blits (copies) the UImg img into obj
 * @param obj UImg object
 * @param img UImg object to copy onto obj
 * @param pos_center of the img
 * @note just copies the values, no blending internally
 *       uses UImg_pos_to_offset_lb and just calls UImg_blit_lb
 */
O_INLINE
void UImg_blit(oobj obj, oobj img, vec2 pos_center)
{
    vec2 lb = UImg_pos_to_offset_lb(obj, pos_center, UImg_size(img));
    UImg_blit_lb(obj, img, ivec2_(lb.x, lb.y));
}


//
// functions that generate UImg's
//

/**
 * Clones the UImg
 * @param obj UImg object
 * @return UImg allocated on obj
 */
O_EXTERN
UImg* UImg_clone(oobj obj);


/**
 * Clones the UImg
 * @param obj UImg object
 * @param format for the new generated UImg
 * @return UImg allocated on obj
 * @note R -> RGBA: vec4_(R, R, R, 1)
 *       RGBA -> R: RGBA.r
 *       32F -> 8 will be clamped
 */
O_EXTERN
UImg* UImg_cast(oobj obj, enum r_format format);


/**
 * @param obj UImg object.
 * @param channel [0-4) for r, g, b, a
 * @return UImg allocated on obj as single channeled image
 * @note asserts r_format_channels(UImg_format(obj)) > channel
 */
O_EXTERN
UImg* UImg_channel(oobj obj, int channel);

/**
 * @param obj UImg object.
 * @param out_rgba_splits red, green, blue, alpha UImg's allocated ob obj (so just an oobj rgba[4] is needed)
 * @note asserts r_format_channels(UImg_format(obj)) > channel
 */
O_INLINE
void UImg_channel_split_rgba(oobj obj, oobj* out_rgba_splits)
{
    for (int i = 0; i < 4; i++)
    {
        out_rgba_splits[i] = UImg_channel(obj, i);
    }
}

/**
 * @param obj UImg object.
 * @param obj_channel [0-4) to set (for r, g, b, a)
 * @param set UImg object to set the channel from
 * @param set_channel [0-4) to get from (for r, g, b, a)
 * @note asserts r_format_channels(UImg_format(obj)) > obj_channel
 *               r_format_channels(UImg_format(set)) > set_channel
 *               r_format_element_is_8(obj) == r_format_element_is_8(set) // so both float is also ok
 *               UImg_num(obj) == UImg_num(set)
 */
O_EXTERN
void UImg_channel_set(oobj obj, int obj_channel, oobj set, int set_channel);

/**
 * Combines all 4 channels into a single image.
 * If a UImg has more than 1 channel, red is used
 * @param r red
 * @param g green
 * @param b blue
 * @param a alpha
 * @return UImg allocated on r (using size of r)
 * @asserts all UImgs share the same format and gba have at least a UImg_num of a
 */
O_EXTERN
UImg* UImg_channel_merge(oobj r, oobj g, oobj b, oobj a);

/**
 * Creates a new copy of the UImg obj, with the given border around it
 * @param obj UImg object
 * @param lrbt border size in left, right, bottom, top (e. g. ivec4_(8) for all to be 8 pixels wide)
 * @param color border color, for example R_TRANSPARENT, or R_BLACK
 * @return UImg allocated on obj
 */
O_EXTERN
UImg* UImg_border(oobj obj, ivec4 lrbt, vec4 color);

/**
 * Creates a collage (2D grid of images)
 * @param srcs n sized list of UImg's
 * @param start position for the first image like LB for Left Bottom
 * @param block either cols or rows (if row_major -> cols)
 * @param margin grid element size is equal to the maximal img size + 2*margin
 * @param bg_color if img size < grid element size
 * @return UImg allocated on srcs[0] (uses also the format of srcs[0])
 */
O_EXTERN
UImg* UImg_collage(const oobj* srcs, int n,
                   enum UImg_start start, int block, bool row_major,
                   ivec2 margin, vec4 bg_color);


/**
 * Creates a distance transform of the given UImg.
 * Uses .r>0 for the distance in general.
 * @param obj UImg object
 * @param full if true, also diagonal checks are done
 * @return UImg allocated on obj, as R_FORMAT_R_8
 */
O_EXTERN
UImg* UImg_distance_transform(oobj obj, bool full);



#endif //U_UIMAGE_H
