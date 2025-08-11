#ifndef _CORE_CANVAS_ANIMATION_CREATOR_INPUTCONTROLLER_H_
#define _CORE_CANVAS_ANIMATION_CREATOR_INPUTCONTROLLER_H_

#include "thorvg.h"
#include "core/input/input.h"
#include "editMode/editMode.h"

namespace core
{

class Scene;
class AnimationCreatorCanvas;
class InputController;

class AnimationCreatorInputController
{
	struct ShapeAddContext
	{
		std::unique_ptr<core::Scene> tempScene;
		tvg::Shape* mNewShape = nullptr;
	};
	struct PathEditContext
	{
		std::vector<tvg::Shape*> mPathPoint;
	};

public:
	using ThisClass = AnimationCreatorInputController;
	AnimationCreatorInputController(AnimationCreatorCanvas* canvas);
	void setMode(EditModeType mode);
	EditModeType getMode()
	{
		return mMode;
	}

	void onStarClickLefttMouse(const InputValue& inputValue);
	void onDragLeftMouse(const InputValue& inputValue);
	void onEndLeftMouse(const InputValue& inputValue);
	void onInputDetach(const InputValue& inputValue);

	std::unique_ptr<InputController> mHandle;

private:
	std::unique_ptr<EditMode> mEditMode;
	EditModeType mMode;
	AnimationCreatorCanvas* rCanvas;
};

}	 // namespace core

#endif