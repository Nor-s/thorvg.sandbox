#include "canvas.h"

namespace core
{

CanvasWrapper::CanvasWrapper(void* context, tvg::Size size) : rContext(context)
{
	mRenderTarget = new GlRenderTarget();
	mRenderTarget->setViewport(tvg::RenderRegion{.min = {0, 0}, .max = {(int) size.x, (int) size.y}});
	mRenderTarget->init(size.x, size.y, 0);

	mCanvas = tvg::GlCanvas::gen();

	resize(size);
}

void CanvasWrapper::draw()
{
	if (mBeforeSize != mSize)
	{
		resize(mSize);
	}
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRenderTarget->getResolveFboId());
		glViewport(0, 0, (int) mSize.x, (int) mSize.y);
		glClearColor(mClearColor[0], mClearColor[1], mClearColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	mCanvas->draw(false);
	mCanvas->sync();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CanvasWrapper::resize(tvg::Size size)
{
	mBeforeSize = mSize;
	mSize = size;

	mRenderTarget->reset();
	mRenderTarget->setViewport(tvg::RenderRegion{.min = {0, 0}, .max = {(int) size.x, (int) size.y}});
	mRenderTarget->init(size.x, size.y, 0);

	mCanvas->target(rContext, mRenderTarget->getResolveFboId(), size.x, size.y, tvg::ColorSpace::ABGR8888S);
}
uint32_t CanvasWrapper::getTexture()
{
	return mRenderTarget->getColorTexture();
}

}	 // namespace core