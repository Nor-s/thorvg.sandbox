#include "glUtil.h"

#include "core/platform.h"

namespace core::gl::util
{

// need  delete[] buffer;
unsigned char* ToBuffer(uint32_t textureId, int width, int height)
{
	unsigned char* buffer = new unsigned char[width * height * 4];	  // RGBA

	glBindTexture(GL_TEXTURE_2D, textureId);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

    return buffer;
}

uint32_t* ToUINTBuffer(uint32_t textureId, int width, int height)
{
	uint32_t* buffer = new uint32_t[width * height * 4];	  // RGBA

	glBindTexture(GL_TEXTURE_2D, textureId);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

    return buffer;
}


}