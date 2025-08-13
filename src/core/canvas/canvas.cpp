#include "canvas.h"

#include "common/common.h"

#include "core/gpu/gl/glUtil.h"
#include "core/gpu/gl/extraGl.h"
#include "core/system/io.h"
#include "core/input/inputController.h"

#include <tvgGlRenderTarget.h>
#include <tvgCanvas.h>

namespace core
{

CanvasWrapper::CanvasWrapper(void* context, Size size, bool bIsSw) : rContext(context), mIsSw(bIsSw)
{
	mRenderTarget = new GlRenderTarget();

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
}

void CanvasWrapper::draw()
{
	mCanvas->update();
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

		// todo: cache buffer
		uint32_t* tempBuffer = new uint32_t[mSize.x * mSize.y];
		for (int y = 0; y < height; ++y)
		{
			uint32_t* srcRow = mSwBuffer + y * stride;
			uint32_t* dstRow = tempBuffer + (height - 1 - y) * stride;

			std::memcpy(dstRow, srcRow, sizeof(uint32_t) * width);
		}
		glBindTexture(GL_TEXTURE_2D, getTexture());

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mSize.x, mSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempBuffer);
		delete[] tempBuffer;
	}
}

void CanvasWrapper::resize(Size size)
{
	mBeforeSize = mSize;
	mSize = size;

	mRenderTarget->reset();
	mRenderTarget->setViewport(tvg::RenderRegion{.min = {0, 0}, .max = {(int) size.x, (int) size.y}});
	mRenderTarget->init(size.x, size.y, 0);

	mCanvas->sync();
	if (mIsSw)
	{
		if (mSwBuffer)
			delete[] mSwBuffer;
		mSwBuffer = new uint32_t[size.x * size.y];
		static_cast<SwCanvas*>(mCanvas)->target(mSwBuffer, size.x, size.x, size.y, tvg::ColorSpace::ABGR8888S);
	}
	else
	{
		static_cast<GlCanvas*>(mCanvas)->target(rContext, mRenderTarget->getResolveFboId(), size.x, size.y,
												tvg::ColorSpace::ABGR8888S);
	}
}

void CanvasWrapper::update()
{
	mCanvas->update();
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

	mBuffer = gl::util::ToBuffer(mRenderTarget->getResolveFboId(), mSize.x, mSize.y);

	return mBuffer;
}

InputController* CanvasWrapper::getInputController()
{
	return nullptr;
}

}	 // namespace core