#include "bbox.h"
#include "input/inputController.h"

#include "scene/scene.h"
#include "scene/component/components.h"

#include "canvas/shapeUtil.h"
#include "controlBox.h"

#include "common/common.h"

namespace core
{

Bbox::Bbox(InputController* inputController, core::Scene* scene, Entity target) : rScene(scene)
{
	rInputController = inputController;
	// 2 is the priority
	mInputActionBindings.push_back(inputController->bindAction(
		InputAction(InputType::MOUSE_LEFT_DOWN, 2, true), InputTrigger::Started, this, &Bbox::onStartClickLeftMouse));
	mInputActionBindings.push_back(inputController->bindAction(InputAction(InputType::MOUSE_LEFT_DOWN, 2, true),
															   InputTrigger::Triggered, this, &Bbox::onDragLeftMouse));
	mInputActionBindings.push_back(inputController->bindAction(InputAction(InputType::MOUSE_LEFT_DOWN, 2, true),
															   InputTrigger::Ended, this, &Bbox::onEndLeftMouse));

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
	if (rTarget.mHandle == entt::null || !rTarget.hasComponent<ShapeComponent>() ||
		mCurrentControlType == ControlTypeCount)
	{
		return;
	}

	retarget(rTarget);
}

void Bbox::retarget(Entity target)
{
	rTarget = target;
	if (rTarget.mHandle == entt::null || !rTarget.hasComponent<ShapeComponent>())
	{
		for (auto& controlBox : mControlBox)
		{
			controlBox.reset();
		}
		mCurrentControlType = ControlTypeCount;
		return;
	}

	auto& targetShape = target.getComponent<ShapeComponent>();
	auto moveAnchorPoint = [this]()
	{
		LOG_INFO("TODO: move AnchorPoint");
		return true;
	};
	auto moveBox = [this]()
	{
		rTarget.moveByDelta(mCurrentPoint - mBeforePoint);
		return true;
	};
	auto scaleLambda = [this]()
	{
        const auto start = mStartPoint;
        const auto current = mCurrentPoint;

        const auto inverse =  mBeforeTransform.inverse();
        const auto localStart = start*inverse;
        const auto localCurrent = current*inverse;
        const auto ratio = (localCurrent/localStart);
        const auto currentScale = Vec2{mBeforeTransform.scale.x * ratio.x, mBeforeTransform.scale.y * ratio.y};

		auto& transform = rTarget.getComponent<TransformComponent>();
		transform.scale = currentScale;

		return true;
	};
	auto rotationLambda = [this]()
	{
		auto& transform = rTarget.getComponent<TransformComponent>();

		const auto pivot = mBeforeTransform.worldPosition;
		auto before = mBeforePoint - pivot; 
		auto current = mCurrentPoint - pivot;

		const auto beforeLen = length(before);
		const auto currentLen = length(current);
		if (beforeLen < 1e-6f || currentLen< 1e-6f)
			return true;

        before = normalize(before);
        current = normalize(current);

		const auto dot = before*current;
		const auto cross = core::cross(before, current);
		const auto rad = std::atan2(cross, dot);
		const auto deg = ToDegree(rad);

		transform.rotation +=  deg;
		return true;
	};
	// todo: case by case - scale, + scale
	// todo: scale mode, dimension mode
	// todo: reset -> hide & show & move
	std::array<Vec2, 4> points = GetObb(targetShape.shape);
	const auto centerPoint = GetCenter(points);
	auto wh = Vec2{Style::BBoxControlBoxWidth, Style::BBoxControlBoxWidth};

	// todo: reset unique_ptr -> move point, change transform box (apply target transform)
	mControlBox[AnchorPoint] =
		std::make_unique<ControlBox>(rScene, centerPoint, wh, ControlBox::Type::Move, ControlBox::ShapeType::Ellipse);

	mControlBox[BoxArea] = std::make_unique<ControlBox>(rScene, points);
	mControlBox[BoxArea]->setOnLeftDrag(MakeLambda(moveBox));

	mControlBox[AnchorPoint]->setOnLeftDrag(MakeLambda(moveAnchorPoint));
	mControlBox[TopLeftScale] = std::make_unique<ControlBox>(rScene, points[0], wh, ControlBox::Type::Scale);
	mControlBox[TopLeftScale]->setOnLeftDrag(MakeLambda(scaleLambda));
	mControlBox[TopRightScale] = std::make_unique<ControlBox>(rScene, points[1], wh, ControlBox::Type::Scale);
	mControlBox[TopRightScale]->setOnLeftDrag(MakeLambda(scaleLambda));
	mControlBox[BottomLeftScale] = std::make_unique<ControlBox>(rScene, points[2], wh, ControlBox::Type::Scale);
	mControlBox[BottomLeftScale]->setOnLeftDrag(MakeLambda(scaleLambda));
	mControlBox[BottomRightScale] = std::make_unique<ControlBox>(rScene, points[3], wh, ControlBox::Type::Scale);
	mControlBox[BottomRightScale]->setOnLeftDrag(MakeLambda(scaleLambda));

	wh = wh * 5.0f;	   // make it bigger for rotate
	mControlBox[TopLeftRotate] =
		std::make_unique<ControlBox>(rScene, points[0], wh, ControlBox::Type::Rotate, ControlBox::ShapeType::Ellipse);
	mControlBox[TopLeftRotate]->setOnLeftDrag(MakeLambda(rotationLambda));
	mControlBox[TopRightRotate] =
		std::make_unique<ControlBox>(rScene, points[1], wh, ControlBox::Type::Rotate, ControlBox::ShapeType::Ellipse);
	mControlBox[TopRightRotate]->setOnLeftDrag(MakeLambda(rotationLambda));
	mControlBox[BottomLeftRotate] =
		std::make_unique<ControlBox>(rScene, points[2], wh, ControlBox::Type::Rotate, ControlBox::ShapeType::Ellipse);
	mControlBox[BottomLeftRotate]->setOnLeftDrag(MakeLambda(rotationLambda));
	mControlBox[BottomRightRotate] =
		std::make_unique<ControlBox>(rScene, points[3], wh, ControlBox::Type::Rotate, ControlBox::ShapeType::Ellipse);
	mControlBox[BottomRightRotate]->setOnLeftDrag(MakeLambda(rotationLambda));

}

bool Bbox::onStartClickLeftMouse(const InputValue& inputValue)
{
	mStartPoint = inputValue.get<Vec2>();
	mCurrentPoint = mBeforePoint = mStartPoint;

	if (mCurrentControlType != ControlTypeCount)
	{
		mCurrentControlType = ControlTypeCount;	   // reset
		return false;
	}

	if (rTarget.mHandle == entt::null)
	{
		return false;
	}

	for (int type = 0; type < ControlTypeCount; type++)
	{
		if (mControlBox[type]->onLeftDown(mStartPoint))
		{
			mBeforeTransform = rTarget.getComponent<TransformComponent>();
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

	if (rTarget.mHandle == entt::null || mCurrentControlType == ControlTypeCount)
	{
		return false;
	}

	return mControlBox[mCurrentControlType]->onLeftDrag();
}
bool Bbox::onEndLeftMouse(const InputValue& inputValue)
{
	// todo: undo/redo event & keyframe
	if (mCurrentControlType == ControlTypeCount)
	{
		return false;
	}
	mCurrentControlType = ControlTypeCount;
	return true;
}

}	 // namespace core