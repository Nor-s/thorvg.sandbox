#include "extraGl.h"

#include <tvgCommon.h>
#ifdef __EMSCRIPTEN__

bool extraGlInit()
{
	return true;
}

#elif defined(__APPLE__) || defined(__MACH__)

PFNGLGETTEXIMAGEPROC glGetTexImage;
PFNGLPIXELSTOREIPROC glPixelStorei;
PFNGLREADBUFFERPROC glReadBuffer;
PFNGLREADPIXELSPROC glReadPixels;

#include <dlfcn.h>
static void* _libGL = nullptr;

static bool _glLoad()
{
	if (!_libGL)
		_libGL = dlopen("/Library/Frameworks/OpenGL.framework/OpenGL", RTLD_LAZY);
	if (!_libGL)
		_libGL = dlopen("/System/Library/Frameworks/OpenGL.framework/OpenGL", RTLD_LAZY);
	if (_libGL)
		return true;
	TVGERR("GL_ENGINE", "Cannot find gl library.");
	return false;
}

static void* _getProcAddress(const char* procName)
{
	return dlsym(_libGL, procName);
}

#define GL_FUNCTION_FETCH(procName, procType)                   \
	procName = (procType) _getProcAddress(#procName);           \
	if (!procName)                                              \
	{                                                           \
		TVGERR("GL_ENGINE", "%s is not supported.", #procName); \
		return false;                                           \
	}

bool extraGlInit()
{
	if (!_glLoad())
		return false;

	GL_FUNCTION_FETCH(glGetTexImage, PFNGLGETTEXIMAGEPROC);
	GL_FUNCTION_FETCH(glPixelStorei, PFNGLPIXELSTOREIPROC);
	GL_FUNCTION_FETCH(glReadBuffer, PFNGLREADBUFFERPROC);
	GL_FUNCTION_FETCH(glReadPixels, PFNGLREADPIXELSPROC);
	return true;
}

#endif
