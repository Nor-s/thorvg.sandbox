#include "canvas.h"

#include <tvgGlRenderTarget.h>
#include <tvgCanvas.h>

#include "core/gpu/gl/glUtil.h"

#include "core/system/io.h"

#include "core/gpu/gl/extraGl.h"

namespace core
{

CanvasWrapper::CanvasWrapper(void* context, tvg::Size size, bool bIsSw) : rContext(context), mIsSw(bIsSw)
{
	mRenderTarget = new GlRenderTarget();
	mRenderTarget->setViewport(tvg::RenderRegion{.min = {0, 0}, .max = {(int) size.x, (int) size.y}});
	mRenderTarget->init(size.x, size.y, 0);

	if (mIsSw)
	{
		mCanvas = tvg::SwCanvas::gen();
	}
	else
	{
		mCanvas = tvg::GlCanvas::gen();
	}
	resize(size);
}
CanvasWrapper::~CanvasWrapper()
{
}

void CanvasWrapper::onUpdate()
{
	if (mBeforeSize != mSize)
	{
		resize(mSize);
		onResize();
	}
	mGlobalElapsed += io::deltaTime * 1000;

	for (auto& anim : mAnimations)
	{
		anim->frame(mGlobalElapsed);
	}
	if (mAnimations.size() > 0)
	{
		mCanvas->update();
	}
}

void CanvasWrapper::draw()
{
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderTarget->getResolveFboId());
		glViewport(0, 0, (int) mSize.x, (int) mSize.y);
		glClearColor(mClearColor[0], mClearColor[1], mClearColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	mCanvas->draw(mIsSw);
	mCanvas->sync();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (mIsSw && mSwBuffer)
	{
		int stride = mSize.x;
		int height = mSize.y;
		int width = mSize.x;

		// todo: optimization, memory allocation
		uint32_t* tempBuffer = new uint32_t[mSize.x * mSize.y];
		for (int y = 0; y < height; ++y)
		{
			uint32_t* srcRow = mSwBuffer + y * stride;
			uint32_t* dstRow = tempBuffer + (height - 1 - y) * stride;

			std::memcpy(dstRow, srcRow, sizeof(uint32_t) * width);
		}
		glBindTexture(GL_TEXTURE_2D, getTexture());

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mSize.x, mSize.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, tempBuffer);
		delete [] tempBuffer;
		// glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void CanvasWrapper::resize(tvg::Size size)
{
	mBeforeSize = mSize;
	mSize = size;

	mRenderTarget->reset();
	mRenderTarget->setViewport(tvg::RenderRegion{.min = {0, 0}, .max = {(int) size.x, (int) size.y}});
	mRenderTarget->init(size.x, size.y, 0);

	if (mIsSw)
	{
		if (mSwBuffer)
			delete[] mSwBuffer;
		mSwBuffer = new uint32_t[size.x* size.y];
		static_cast<SwCanvas*>(mCanvas)->target(mSwBuffer, size.x, size.x, size.y, tvg::ColorSpace::ARGB8888);
	}
	else
	{
		static_cast<GlCanvas*>(mCanvas)->target(rContext, mRenderTarget->getResolveFboId(), size.x, size.y,
												tvg::ColorSpace::ABGR8888S);
	}
}

uint32_t CanvasWrapper::getTexture()
{
	return mRenderTarget->getColorTexture();
}

unsigned char* CanvasWrapper::getBuffer()
{
	if (mBuffer != nullptr)
	{
		delete[] mBuffer;
		mBuffer = nullptr;
	}
	mBuffer = gl::util::ToBuffer(getTexture(), mSize.x, mSize.y);

	return mBuffer;
}

}	 // namespace core