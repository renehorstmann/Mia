#ifndef R_GL_H
#define R_GL_H

/**
 * @file gl.h
 *
 * Include this file to include OpenGL functions
 */

#include "common.h"


#define GL_GLEXT_PROTOTYPES

#ifdef MIA_OPTION_GLEW
#include <GL/glew.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#endif //R_GL_H
