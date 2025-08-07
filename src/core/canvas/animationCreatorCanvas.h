#ifndef _CORE_CANVAS_ANIMATION_CREATOR_CANVAS_H_
#define _CORE_CANVAS_ANIMATION_CREATOR_CANVAS_H_

#include "canvas.h"

namespace core
{

class Scene;
class AnimationBuilder;
class AnimationCreatorInputController;

class AnimationCreatorCanvas : public CanvasWrapper
{
public:
	AnimationCreatorCanvas(void* context, Size size, bool bIsSw);
	~AnimationCreatorCanvas() = default;

	CanvasType type() override
	{
		return CanvasType::LottieCreator;
	}

	AnimationBuilder* builder()
	{
		return mBuilder.get();
	}
	InputController* getInputController() override;

	std::unique_ptr<core::Scene> mScene;
	std::unique_ptr<AnimationCreatorInputController> mInputController;

private:
	std::unique_ptr<AnimationBuilder> mBuilder;
};

}	 // namespace core

#endif