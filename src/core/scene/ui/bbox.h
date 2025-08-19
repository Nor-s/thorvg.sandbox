#ifndef _CORE_CANVAS_EDITMODE_BBOX_H_
#define _CORE_CANVAS_EDITMODE_BBOX_H_

#include "scene/entity.h"
#include "scene/component/components.h"

#include <array>

namespace core
{

class InputActionBinding;
class InputController;
class InputValue;
class ControlBox;
class Scene;

class Bbox
{
	enum ControlType
	{
		BoxArea = 0,
		AnchorPoint,
		TopLeftScale,
		TopRightScale,
		BottomLeftScale,
		BottomRightScale,
		// TopCenterScale,
		// BottomCenterScale,
		// LeftCenterScale,
		// RightCenterScale,
		TopLeftRotate,
		TopRightRotate,
		BottomLeftRotate,
		BottomRightRotate,
		ControlTypeCount
	};

public:
	Bbox(InputController* inputController, core::Scene* scene, Entity target);
	~Bbox();

	void retarget(Entity target);

	void onUpdate();
	bool onStartClickLeftMouse(const InputValue& inputValue);
	bool onDragLeftMouse(const InputValue& inputValue);
	bool onEndLeftMouse(const InputValue& inputValue);

private:
	Entity rTarget;
	Scene* rScene{nullptr};
	TransformComponent mBeforeTransform;
	InputController* rInputController{nullptr};
	Vec2 mStartPoint{0.0f, 0.0f};
	Vec2 mBeforePoint{0.0f, 0.0f};
	Vec2 mCurrentPoint{0.0f, 0.0f};
	ControlType mCurrentControlType{ControlTypeCount};
	std::array<std::unique_ptr<ControlBox>, ControlTypeCount> mControlBox;
	std::vector<InputActionBinding*> mInputActionBindings;
};

}	 // namespace core

#endif