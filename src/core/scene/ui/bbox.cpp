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

// must update after target entity update
void Bbox::onUpdate()
{
    if (rTarget.mHandle == entt::null || !rTarget.hasComponent<ShapeComponent>() || mCurrentControlType == ControlTypeCount)
    {
        return;
    }

    retarget(rTarget);
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
    // todo: scale mode, dimension mode
    // todo: reset -> hide & show & move
    std::array<Vec2, 4> points = GetObb(targetShape.shape);
    Vec2 centerPoint = GetCenter(points);
    Vec2 wh = Vec2{8.0f, 8.0f};
    mControlBox[AnchorPoint] = std::make_unique<ControlBox>(rScene, centerPoint, wh, ControlBox::Type::Move, ControlBox::ShapeType::Ellipse);
    mControlBox[AnchorPoint]->setOnLeftDrag(MakeLambda([this]() {
        LOG_INFO("AnchorPoint");
        return false;
    }));
    mControlBox[BoxArea] = std::make_unique<ControlBox>(rScene, points);
    mControlBox[BoxArea]->setOnLeftDrag(MakeLambda([this]() {
        LOG_INFO("Box Area Drag");
        return true;
    }));
    mControlBox[TopLeftScale] = std::make_unique<ControlBox>(rScene, points[0], wh, ControlBox::Type::Scale);
    mControlBox[TopLeftScale]->setOnLeftDrag(MakeLambda([this]() {
        LOG_INFO("TopLeftScale onLeftDrag");
        return true;
    }));
    mControlBox[TopRightScale] = std::make_unique<ControlBox>(rScene, points[1], wh, ControlBox::Type::Scale);
    mControlBox[BottomLeftScale] = std::make_unique<ControlBox>(rScene, points[2], wh, ControlBox::Type::Scale);
    mControlBox[BottomRightScale] = std::make_unique<ControlBox>(rScene, points[3], wh, ControlBox::Type::Scale);


    wh = wh*5.0f; // make it bigger for rotate
    mControlBox[TopLeftRotate] = std::make_unique<ControlBox>(rScene, points[0], wh, ControlBox::Type::Rotate, ControlBox::ShapeType::Ellipse);
    mControlBox[TopRightRotate] = std::make_unique<ControlBox>(rScene, points[1], wh, ControlBox::Type::Rotate, ControlBox::ShapeType::Ellipse);
    mControlBox[BottomLeftRotate] = std::make_unique<ControlBox>(rScene, points[2], wh, ControlBox::Type::Rotate, ControlBox::ShapeType::Ellipse);
    mControlBox[BottomRightRotate] = std::make_unique<ControlBox>(rScene, points[3], wh, ControlBox::Type::Rotate, ControlBox::ShapeType::Ellipse);
    mCurrentScale = Vec2{1.0f, 1.0f};
}

bool Bbox::onStartClickLeftMouse(const InputValue& inputValue)
{
    mStartPoint = inputValue.get<Vec2>();
    mCurrentPoint = mBeforePoint = mStartPoint;
    for(int type = AnchorPoint; type < ControlTypeCount; type++)
    {
        if(mControlBox[type]->onLeftDown(mStartPoint))
        {
            mCurrentControlType = ControlType(type);
            return true;
        }
    }
    mCurrentControlType = ControlTypeCount;
	return false;
}
bool Bbox::onDragLeftMouse(const InputValue& inputValue)
{
    mBeforePoint = mCurrentPoint;
    mCurrentPoint = inputValue.get<Vec2>();
    if (mCurrentControlType != ControlTypeCount)
    {
        return mControlBox[mCurrentControlType]->onLeftDrag();
    }

    return false;
}
bool Bbox::onEndLeftMouse(const InputValue& inputValue)
{

    mCurrentControlType = ControlTypeCount;
    return false;
}


}	 // namespace core