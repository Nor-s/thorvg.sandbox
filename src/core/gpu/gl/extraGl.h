#ifndef _CORE_GPU_GL_EXTRA_GL_H
#define _CORE_GPU_GL_EXTRA_GL_H

#include <tvgGl.h>

typedef void (*PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void (*PFNGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);

extern PFNGLGETTEXIMAGEPROC glGetTexImage;
extern PFNGLPIXELSTOREIPROC glPixelStorei;

bool extraGlInit();

#endif