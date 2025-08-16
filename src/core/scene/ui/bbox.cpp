#include "bbox.h"
#include "input/inputController.h"

#include "scene/scene.h"
#include "scene/component/components.h"

#include "canvas/shapeUtil.h"
#include "controlBox.h"

#include "common/common.h"

namespace core
{

Bbox::Bbox(InputController* inputController, core::Scene* scene, Entity target) 
    : rScene(scene)
{
    rInputController = inputController;
    // 2 is the priority
	mInputActionBindings.push_back(inputController->bindAction(
		InputAction(InputType::MOUSE_LEFT_DOWN, 2, true), InputTrigger::Started, this, &Bbox::onStartClickLeftMouse));
	mInputActionBindings.push_back(inputController->bindAction(
		InputAction(InputType::MOUSE_LEFT_DOWN, 2, true), InputTrigger::Triggered, this, &Bbox::onDragLeftMouse));
	mInputActionBindings.push_back(inputController->bindAction(
		InputAction(InputType::MOUSE_LEFT_DOWN, 2, true), InputTrigger::Ended, this, &Bbox::onEndLeftMouse));

    retarget(target);
}

Bbox::~Bbox()
{
    for (auto& binding : mInputActionBindings)
    {
        rInputController->unbinding(binding);
    }
}

void Bbox::retarget(Entity target)
{
	rTarget = target;
    if(rTarget.mHandle == entt::null || !rTarget.hasComponent<ShapeComponent>())
    {
        return;
    }

    auto& targetShape = target.getComponent<ShapeComponent>();
    // todo: case by case - scale, + scale
    // todo: reset -> hide & show & move
    std::array<Vec2, 4> points = GetObb(targetShape.shape);
    Vec2 wh = Vec2{8.0f, 8.0f};
    mControlBox[TopLeftScale] = std::make_unique<ControlBox>(rScene, points[0], wh, ControlBox::Type::Scale);
    mControlBox[TopLeftScale]->setOnLeftDrag(MakeLambda([this]() {
        LOG_INFO("TopLeftScale onLeftDrag");
        return true;
    }));
    mControlBox[TopRightScale] = std::make_unique<ControlBox>(rScene, points[1], wh, ControlBox::Type::Scale);
    mControlBox[BottomLeftScale] = std::make_unique<ControlBox>(rScene, points[2], wh, ControlBox::Type::Scale);
    mControlBox[BottomRightScale] = std::make_unique<ControlBox>(rScene, points[3], wh, ControlBox::Type::Scale);
}

bool Bbox::onStartClickLeftMouse(const InputValue& inputValue)
{
	return false;
}
bool Bbox::onDragLeftMouse(const InputValue& inputValue)
{
    return false;
}
bool Bbox::onEndLeftMouse(const InputValue& inputValue)
{
    return false;
}


}	 // namespace core