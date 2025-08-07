#include "animationCreatorInputController.h"
#include "canvas.h"
#include "animationCreatorCanvas.h"

#include "input/input.h"
#include "input/inputAction.h"
#include "input/inputController.h"

#include "common/common.h"

namespace core
{

AnimationCreatorInputController::AnimationCreatorInputController(AnimationCreatorCanvas* canvas)
{
	rCanvas = canvas;

	mHandle = std::make_unique<InputController>();

	mHandle->bindAction(InputAction(InputType::MOUSE_LEFT_DOWN), InputTrigger::Started, this,
						&ThisClass::onStarClickLefttMouse);
	mHandle->bindAction(InputAction(InputType::MOUSE_LEFT_DOWN), InputTrigger::Triggered, this,
						&ThisClass::onDragLeftMouse);
	mHandle->bindAction(InputAction(InputType::MOUSE_LEFT_DOWN), InputTrigger::Ended, this,
						&ThisClass::onEndLeftMouse);
}

void AnimationCreatorInputController::setMode(EditMode mode)
{
    LOG_INFO("change edit mode: {}", (int)mMode);
	mMode = mode;
}

void AnimationCreatorInputController::onStarClickLefttMouse(const InputValue& inputValue)
{
    LOG_INFO("start edit: mode is {}", (int)mMode);
    
	switch (mMode)
	{
		case EditMode::PICK:
		{
			mInputCB = std::make_unique<PickCB>();
			mInputCB->onStarClickLefttMouse(inputValue);
			break;
		}
		case EditMode::ADD_SQUARE:
		case EditMode::ADD_ELLIPSE:
		case EditMode::ADD_POLYGON:
		case EditMode::ADD_STAR:
		{
			// TODO
			mInputCB = nullptr;
			break;
		}
		case EditMode::ADD_PEN_PATH:
		{
			// TODO
			mInputCB = nullptr;
			break;
		}
		case EditMode::EDIT_PEN_PATH:
		{
			// TODO
			mInputCB = nullptr;
			break;
		}
	};
}

void AnimationCreatorInputController::onDragLeftMouse(const InputValue& inputValue)
{
    LOG_INFO("trigger edit: mode is {}", (int)mMode);
}

void AnimationCreatorInputController::onEndLeftMouse(const InputValue& inputValue)
{
    LOG_INFO("end edit: mode is {}", (int)mMode);
}

}	 // namespace core