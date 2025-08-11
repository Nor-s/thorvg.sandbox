#include "pickMode.h"
#include "../canvas.h"
#include "../animationCreatorCanvas.h"

namespace core
{

PickMode::PickMode(AnimationCreatorCanvas* canvas)
{
	rCanvas = canvas;
	mContext.tempScene = tvg::Scene::gen();
	rCanvas->getCanvas()->push(mContext.tempScene);
}
PickMode::~PickMode()
{
	rCanvas->getCanvas()->remove(mContext.tempScene);
}
void PickMode::onStarClickLefttMouse(const InputValue& inputValue)
{
	if (mContext.mPick == nullptr)
	{
		// pick?
	}

	if (mContext.mPick)
	{
	}
}
void PickMode::onDragLeftMouse(const InputValue& inputValue)
{
}
void PickMode::onEndLeftMouse(const InputValue& inputValue)
{
}
void PickMode::onInputDetach(const InputValue& inputValue)
{
}
}	 // namespace core