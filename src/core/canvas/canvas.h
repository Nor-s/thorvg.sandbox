#ifndef _CORE_CANVAS_CANVAS_H_
#define _CORE_CANVAS_CANVAS_H_

#include "paintWrapper.h"

// todo: clean header
#include <thorvg.h>

#include <vector>
#include <memory>


class GlRenderTarget;

namespace core
{

// todo: delete move, copy
class CanvasWrapper
{
public:
	CanvasWrapper(void* context, tvg::Size size, bool bIsSw);
	virtual ~CanvasWrapper();

	void clearColor(float color[3])
	{
		memcpy(mClearColor, color, 3 * sizeof(float));
	}

	virtual void onInit() {};
	virtual void onUpdate();
	virtual void onResize() {};
	virtual bool isExampleCanvas() {return false;}

	void draw();
	void resize(tvg::Size size);
	uint32_t getTexture();

	tvg::Canvas* getCanvas()
	{
		return mCanvas;
	}
	unsigned char* getBuffer();

	void pushPaint(std::unique_ptr<PaintWrapper> paint)
	{
		paint->scale(mSize);
		mCanvas->push(paint->mHandle);
		mCanvas->update();

		mPaints.push_back(std::move(paint));
	}

	void pushAnimation(std::unique_ptr<AnimationWrapper> anim)
	{
		mAnimations.push_back(std::move(anim));
	}

	bool isSw()
	{
		return mIsSw;
	}

	tvg::Size mSize{};

protected:
	// todo: smart pointer
	GlRenderTarget* mRenderTarget{};
	tvg::Canvas* mCanvas{nullptr};
	std::vector<std::unique_ptr<PaintWrapper>> mPaints;
	std::vector<std::unique_ptr<AnimationWrapper>> mAnimations;

	float mClearColor[3]{};
	void* rContext{nullptr};
	tvg::Size mBeforeSize;
	uint32_t mGlobalElapsed = 0;
	bool mIsSw = false;

	unsigned char* mBuffer = nullptr;
	uint32_t* mSwBuffer = nullptr;
};

}	 // namespace core

#endif