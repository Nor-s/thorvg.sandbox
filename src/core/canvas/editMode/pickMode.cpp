#include "pickMode.h"

#include "../canvas.h"
#include "../animationCreatorCanvas.h"

#include "scene/scene.h"
#include "scene/component/components.h"
#include "scene/ui/bbox.h"

namespace core
{

PickMode::PickMode(AnimationCreatorCanvas* canvas)
{
	rCanvas = canvas;

	mContext.tempScene = std::make_unique<core::Scene>();
	mContext.tempScene->pushCanvas(rCanvas);
	mContext.pickInfo.excludeIds.insert(mContext.tempScene->mId);

	rCanvas->getCanvas()->push(mContext.tempScene->getScene());
}

PickMode::~PickMode()
{
	rCanvas->getCanvas()->remove(mContext.tempScene->getScene());
}

void PickMode::onUpdate()
{
	if (mContext.tempScene)
	{
		if(mContext.bbox)
		{
			mContext.bbox->onUpdate();
		}
		mContext.tempScene->onUpdate();
	}
}

bool PickMode::onStarClickLefttMouse(const InputValue& inputValue)
{
	mContext.isLeftMouseDown = true;
	mContext.startPoint = inputValue.get<Vec2>();
	mContext.beforePoint = mContext.startPoint;

	bool isPick = Pick(rCanvas->getCanvas(), mContext.pickInfo, mContext.startPoint);

	if (isPick && mContext.pickInfo.currentSelectedScene)
	{
		if(mContext.bbox == nullptr)
		{
			mContext.bbox = std::make_unique<Bbox>(rCanvas->getInputController(), mContext.tempScene.get(), 
				mContext.pickInfo.currentSelectedScene->getEntityById(mContext.pickInfo.currentSelectedPaint->id));
		}
		else
		{
			mContext.bbox->retarget(
				mContext.pickInfo.currentSelectedScene->getEntityById(mContext.pickInfo.currentSelectedPaint->id));
			return mContext.bbox->onStartClickLeftMouse(inputValue);
		}
	}
	else
	{
		if(mContext.bbox)
		{
			mContext.bbox->retarget(Entity());
		}
		mContext.pickInfo.currentSelectedPaint = nullptr;
		mContext.pickInfo.currentSelectedScene = nullptr;
	}
	return true;
}
bool PickMode::onDragLeftMouse(const InputValue& inputValue)
{
	if(mContext.bbox)
		return mContext.bbox->onDragLeftMouse(inputValue);
	// if (mContext.isLeftMouseDown == false)
	// 	return;

	// auto endPoint = inputValue.get<Vec2>();
	// auto startPoint = mContext.startPoint;
	// auto start = Vec2{std::min(startPoint.x, endPoint.x), std::min(startPoint.y, endPoint.y)};
	// auto end = Vec2{std::max(startPoint.x, endPoint.x), std::max(startPoint.y, endPoint.y)};
	// auto delta = endPoint - mContext.beforePoint;
	// mContext.beforePoint = endPoint;

	// if (mContext.drag.mHandle != entt::null)
	// 	mContext.tempScene->destroyEntity(mContext.drag);

	// if (mContext.currentSelectedPaint)
	// {
	// 	auto id = mContext.currentSelectedPaint->id;
	// 	auto entity = mContext.currentSelectedScene->getEntityById(id);
	// 	entity.moveByDelta(delta);
	// 	mContext.bbox.moveByDelta(delta);
	// 	return;
	// }

	// // drag process
	// mContext.drag = mContext.tempScene->createRectFillStrokeLayer("drag", start, end - start);
	// auto& fill = mContext.drag.getComponent<SolidFillComponent>();
	// fill.alpha *= 0.3f;
	return true;
}
bool PickMode::onEndLeftMouse(const InputValue& inputValue)
{
	// if (mContext.isLeftMouseDown == false)
	// 	return;
	// if (mContext.drag.mHandle != entt::null)
	// 	mContext.tempScene->destroyEntity(mContext.drag);
	// mContext.isLeftMouseDown = false;
	return true;
}
bool PickMode::onMoveMouse(const InputValue& inputValue)
{
	return true;
	// if (mContext.hover.mHandle != entt::null)
	// 	mContext.tempScene->destroyEntity(mContext.hover);
	// if (mContext.isLeftMouseDown)
	// 	return;
	// auto& paints = rCanvas->getCanvas()->paints();
	// bool isPicked = false;
	// for (auto& paint : paints)
	// {
	// 	if (paint->id == mContext.tempScene->mId)
	// 		continue;
	// 	isPicked |= pick(inputValue, paint, 0);
	// }
	// if (isPicked && mContext.currentSelectedScene)
	// {
	// 	std::array<Vec2, 4> points = GetObb(mContext.currentSelectedPaint);
	// 	mContext.hover = mContext.tempScene->createObb(points);
	// 	LOG_INFO("hover: {}", mContext.hover.getComponent<IDComponent>().id);
	// }
}
bool PickMode::onInputDetach(const InputValue& inputValue)
{
	mContext.isLeftMouseDown = false;
	return false;
}
}	 // namespace core