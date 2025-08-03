#ifndef _CORE_GPU_GL_EXTRA_GL_H
#define _CORE_GPU_GL_EXTRA_GL_H

#include <tvgGl.h>

#ifdef __EMSCRIPTEN__


#elif defined(__APPLE__) || defined(__MACH__)

typedef void (*PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void (*PFNGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
typedef void (*PFNGLREADBUFFERPROC)(GLenum src);
typedef void (
	*PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);

extern PFNGLREADBUFFERPROC glReadBuffer;
extern PFNGLREADPIXELSPROC glReadPixels;
extern PFNGLGETTEXIMAGEPROC glGetTexImage;
extern PFNGLPIXELSTOREIPROC glPixelStorei;

#endif

bool extraGlInit();

#endif