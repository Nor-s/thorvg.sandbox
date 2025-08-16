#ifndef _CORE_CANVAS_EDITMODE_BBOX_H_
#define _CORE_CANVAS_EDITMODE_BBOX_H_

#include "scene/entity.h"

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
    enum ControlPoint
    {
        TopLeftScale = 0,
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
        ControlPointCount
    };
public:
	Bbox(InputController* inputController, core::Scene* scene, Entity target);
	~Bbox();

    void retarget(Entity target);

    bool onStartClickLeftMouse(const InputValue& inputValue);
    bool onDragLeftMouse(const InputValue& inputValue);
    bool onEndLeftMouse(const InputValue& inputValue);

private:
    Entity rTarget;
    Scene* rScene{nullptr};
    InputController* rInputController{nullptr};
    std::array<std::unique_ptr<ControlBox>, ControlPointCount> mControlBox;
    std::vector<InputActionBinding*> mInputActionBindings;

};

}	 // namespace core

#endif