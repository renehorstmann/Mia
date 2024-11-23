#include "o/img.h"
#include "o/OObj.h"
#include "o/OObjRoot.h"
#include <SDL2/SDL_image.h>

#define O_LOG_LIB "o"
#include "o/log.h"


O_STATIC
bool format_equals(const SDL_PixelFormat *a, const SDL_PixelFormat *b)
{
    return a->BitsPerPixel != b->BitsPerPixel
        || a->Rmask != b->Rmask
        || a->Gmask != b->Gmask
        || a->Bmask != b->Bmask
        || a->Amask != b->Amask;
}


struct o_img o_img_new(oobj obj, int cols, int rows)
{
    struct o_img self = {
            NULL,
            cols, rows,
            obj
    };
    self.rgba_data = o_new(obj, obyte, o_img_data_size(self));
    return self;
}

struct o_img o_img_new0(oobj obj, int cols, int rows)
{
    struct o_img self = o_img_new(obj, cols, rows);
    o_clear(self.rgba_data, 1, o_img_data_size(self));
    return self;
}

struct o_img o_img_new_file(oobj obj, const char *file)
{
    struct o_img self = {0};
    SDL_Surface *img = IMG_Load(file);
    if(!img || img->w <= 0 || img->h <= 0) {
        o_log_warn_s(__func__, "failed to load the image: %s", file);
        return self;
    }

    SDL_PixelFormat *sf = img->format;
    SDL_PixelFormat *df = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);

    // check if the format equals the needed
    //    and convert it otherwise
    if (sf->BitsPerPixel != df->BitsPerPixel
        || sf->Rmask != df->Rmask
        || sf->Gmask != df->Gmask
        || sf->Bmask != df->Bmask
        || sf->Amask != df->Amask) {
        SDL_Surface *tmp = SDL_ConvertSurface(img, df, 0);
        SDL_FreeSurface(img);
        img = tmp;
    }

    if (img->pitch != img->w * 4) {
        o_log_warn_s(__func__, "failed: pitch must be == cols*4: %s\", file");
        goto CLEAN_UP;
    }

    SDL_PixelFormat *f = img->format;
    if(f->BitsPerPixel != 32 || f->Amask == 0) {
        o_log_warn_s(__func__,
                   "failed to load and cast the image, 8bpp and alpha needed: %s", file);
    } else {
        struct o_img flip = {
                img->pixels,
                img->w, img->h,
                obj
        };
        self = flip;
        self.rgba_data = o_new(obj, obyte, o_img_data_size(self));
        o_img_flip_rows(self, flip);
    }

    CLEAN_UP:
    SDL_FreeSurface(img);
    SDL_FreeFormat(df);
    return self;
}

void o_img_move(struct o_img *self, oobj into)
{
    o_mem_move(self->parent, into, self->rgba_data);
    self->parent = into;
}

void o_img_free(struct o_img *self)
{
    if(!self->parent) {
        return;
    }
    o_free(self->parent, self->rgba_data);
    *self = (struct o_img) {0};
}

void *o_img_as_sdl_surface(struct o_img self)
{
    if(o_img_data_size(self) <= 0) {
        o_log_error_s("o_img_write", "failed, data_size <= 0");
        return NULL;
    }
    SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void *) self.rgba_data,
                                                    self.cols,
                                                    self.rows,
                                                    32, self.cols * 4,
                                                    format->Rmask,
                                                    format->Gmask,
                                                    format->Bmask,
                                                    format->Amask);
    SDL_FreeFormat(format);
    if(!surface) {
        o_log_error_s("o_img_write", "failed to create sdl buffer: %s", SDL_GetError());
    }
    return surface;
}

bool o_img_write_file(struct o_img self, const char *file)
{
    oobj root = OObjRoot_new_heap();

    bool valid = false;
    struct o_img flip = self;
    flip.rgba_data = o_new(root, obyte, o_img_data_size(self));
    o_img_flip_rows(flip, self);
    SDL_Surface *surface = o_img_as_sdl_surface(flip);
    if(!surface) {
        goto CLEAN_UP;
    }
    valid = IMG_SavePNG(surface, file) == 0;

    CLEAN_UP:
    SDL_FreeSurface(surface);
    o_del(root);
    if(!valid) {
        o_log_error_s(__func__, "failed to write image %s: %s", file, SDL_GetError());
    }
    return valid;
}



void o_img_flip_rows(struct o_img out, struct o_img src)
{
    if(!src.rgba_data || src.cols<=0 || src.rows <= 0) {
        // silent return if src is invalid
        return;
    }
    assert(out.rgba_data && "must have been allocated");
    assert(out.cols == src.cols && out.rows && src.rows && "sizes must match");
    for(int r=0; r<out.rows; r++) {
        o_memcpy(o_img_at(out, 0, out.rows-r-1),
               o_img_at(src, 0, r), 4,
               out.cols);
    }
}
