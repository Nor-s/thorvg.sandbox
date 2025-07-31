#ifndef _CORE_CANVAS_CANVAS_H_
#define _CORE_CANVAS_CANVAS_H_

// todo: clean header
#include <tvgGl.h>
#include <thorvg.h>
#include <tvgCommon.h>
#include <tvgGlRenderTarget.h>
#include <tvgMath.h>

namespace core
{

// todo: delete move, copy
class CanvasWrapper
{
public:
	CanvasWrapper(void* context, tvg::Size size);

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

	tvg::Size mSize{};

protected:
	// todo: smart pointer
	GlRenderTarget* mRenderTarget{};
	tvg::GlCanvas* mCanvas{nullptr};
	float mClearColor[3]{};
	void* rContext{nullptr};
	tvg::Size mBeforeSize;
};

}	 // namespace core

#endif