#include "animationCreatorInputController.h"
#include "canvas.h"
#include "animationCreatorCanvas.h"

#include "input/input.h"
#include "input/inputAction.h"
#include "input/inputController.h"

#include "common/common.h"

#include "editMode/pickMode.h"
#include "editMode/addMode.h"

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
	mHandle->bindAction(InputAction(InputType::MOUSE_LEFT_DOWN), InputTrigger::Ended, this, &ThisClass::onEndLeftMouse);
	mHandle->bindAction(InputAction(InputType::INPUT_DETACH), InputTrigger::Triggered, this, &ThisClass::onInputDetach);
}

void AnimationCreatorInputController::setMode(EditModeType mode)
{
	LOG_INFO("change edit mode: {}", (int) mMode);
	mMode = mode;
	mEditMode = nullptr;
}

void AnimationCreatorInputController::onStarClickLefttMouse(const InputValue& inputValue)
{
	LOG_INFO("start edit: mode is {}", (int) mMode);

	switch (mMode)
	{
		case EditModeType::PICK:
		{
			if(mEditMode == nullptr)
				mEditMode = std::make_unique<PickMode>(rCanvas);
			break;
		}
		case EditModeType::ADD_SQUARE:
		case EditModeType::ADD_ELLIPSE:
		case EditModeType::ADD_POLYGON:
		case EditModeType::ADD_STAR:
		{
			// TODO
			if(mEditMode == nullptr)
				mEditMode = std::make_unique<AddMode>(rCanvas, mMode);
			break;
		}
		case EditModeType::ADD_PEN_PATH:
		{
			// TODO
			mEditMode = nullptr;
			break;
		}
		case EditModeType::EDIT_PEN_PATH:
		{
			// TODO
			mEditMode = nullptr;
			break;
		}
	};

	if(mEditMode)
	{
		mEditMode->onStarClickLefttMouse(inputValue);
	}
}

void AnimationCreatorInputController::onDragLeftMouse(const InputValue& inputValue)
{
	LOG_INFO("trigger edit: mode is {}", (int) mMode);
	if (mEditMode == nullptr)
		return;
	mEditMode->onDragLeftMouse(inputValue);
}

void AnimationCreatorInputController::onEndLeftMouse(const InputValue& inputValue)
{
	LOG_INFO("end edit: mode is {}", (int) mMode);
	if (mEditMode == nullptr)
		return;
	mEditMode->onEndLeftMouse(inputValue);
}
void AnimationCreatorInputController::onInputDetach(const InputValue& inputValue)
{
	LOG_INFO("detach input");
	if (mEditMode)
		mEditMode->onInputDetach(inputValue);
	mEditMode = nullptr;
}


}	 // namespace core