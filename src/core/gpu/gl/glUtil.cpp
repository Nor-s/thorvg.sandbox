#include "glUtil.h"

#include "core/platform.h"

namespace core::gl::util
{

// need  delete[] buffer;
unsigned char* ToBuffer(uint32_t fbo, int width, int height)
{
	unsigned char* buffer = new unsigned char[width * height * 4];	  // RGBA

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0); 
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return buffer;
}



}