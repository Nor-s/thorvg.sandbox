#include "animationCreatorCanvas.h"

#include "animation/animationBuilder.h"
#include "scene/scene.h"
#include <memory>

#include "core/input/inputController.h"
#include "core/input/inputAction.h"
#include "animationCreatorInputController.h"

namespace core
{

AnimationCreatorCanvas::AnimationCreatorCanvas(void* context, Size size, bool bIsSw)
	: CanvasWrapper(context, size, bIsSw)
{
	mBuilder = std::make_unique<AnimationBuilder>();
	mScene = std::make_unique<core::Scene>();
	mScene->pushCanvas(this);
	mCanvas->push(mScene->mTvgScene);
	mInputController = std::make_unique<AnimationCreatorInputController>(this);
}

InputController* AnimationCreatorCanvas::getInputController()
{
	if (mInputController)
		return mInputController->mHandle.get();
	return nullptr;
}

}	 // namespace core