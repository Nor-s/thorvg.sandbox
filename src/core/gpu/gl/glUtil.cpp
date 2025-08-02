#include "glUtil.h"

#include "core/platform.h"

namespace core::gl::util
{

// need  delete[] buffer;
unsigned char* ToBuffer(uint32_t textureId, int width, int height, bool bIsRGBA)
{
	unsigned char* buffer = new unsigned char[width * height * 4];	  // RGBA

	glBindTexture(GL_TEXTURE_2D, textureId);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    return buffer;
}

}