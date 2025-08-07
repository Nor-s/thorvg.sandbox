#ifndef _CORE_CANVAS_ANIMATION_CREATOR_INPUTCONTROLLER_H_
#define _CORE_CANVAS_ANIMATION_CREATOR_INPUTCONTROLLER_H_

#include "thorvg.h"
#include "core/input/input.h"

namespace core
{

class Scene;
class AnimationCreatorCanvas;
class InputController;

enum class EditMode
{
	NONE,
	PICK,
	ADD_SQUARE,
	ADD_ELLIPSE,
	ADD_POLYGON,
	ADD_STAR,
	ADD_PEN_PATH,
	EDIT_PEN_PATH
};

class InputCB
{
public:
	virtual void onStarClickLefttMouse(const InputValue& inputValue) = 0;
	virtual void onDragLeftMouse(const InputValue& inputValue) = 0;
	virtual void onEndLeftMouse(const InputValue& inputValue) = 0;
};

class PickCB : public InputCB
{
	struct PickContext
	{
		std::vector<tvg::Paint*> rPickedPaint;
		// for outline
		tvg::Shape* mTempScene{nullptr};
		tvg::Shape* mOutline{nullptr};
		tvg::Shape* mScalePoint[4]{nullptr, nullptr, nullptr, nullptr};
	};

public:
    PickCB() = default;
    ~PickCB() = default;
	void onStarClickLefttMouse(const InputValue& inputValue){}
	void onDragLeftMouse(const InputValue& inputValue){}
	void onEndLeftMouse(const InputValue& inputValue){}

private:
	AnimationCreatorCanvas* rCanvas;
	PickContext pickContext;
};

class AnimationCreatorInputController
{
	struct ShapeAddContext
	{
		std::unique_ptr<core::Scene> mTempScene;
		tvg::Shape* mNewShape = nullptr;
	};
	struct PathEditContext
	{
		std::vector<tvg::Shape*> mPathPoint;
	};

public:
	using ThisClass = AnimationCreatorInputController;
	AnimationCreatorInputController(AnimationCreatorCanvas* canvas);
	void setMode(EditMode mode);
	EditMode getMode()
	{
		return mMode;
	}

	void onStarClickLefttMouse(const InputValue& inputValue);
	void onDragLeftMouse(const InputValue& inputValue);
	void onEndLeftMouse(const InputValue& inputValue);

	std::unique_ptr<InputController> mHandle;

private:
	std::unique_ptr<InputCB> mInputCB;
	EditMode mMode;
	AnimationCreatorCanvas* rCanvas;
};

}	 // namespace core

#endif