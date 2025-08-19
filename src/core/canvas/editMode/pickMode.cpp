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
		if (mContext.bbox)
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
		if (mContext.bbox == nullptr)
		{
			mContext.bbox = std::make_unique<Bbox>(
				rCanvas->getInputController(), mContext.tempScene.get(),
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
		if (mContext.bbox)
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
	if (mContext.bbox)
		return mContext.bbox->onDragLeftMouse(inputValue);
	return true;
}
bool PickMode::onEndLeftMouse(const InputValue& inputValue)
{
	return true;
}
bool PickMode::onMoveMouse(const InputValue& inputValue)
{
	// todo: destroy/create -> hide/show
	if (!mContext.hover.isNull())
	{
		mContext.tempScene->destroyEntity(mContext.hover);
	}

	if (mContext.bbox && mContext.bbox->onMoveMouse(inputValue))
	{
		return true;
	}
	PickInfo pickInfo;
	pickInfo.excludeIds.insert(mContext.tempScene->mId);

	bool isPick = Pick(rCanvas->getCanvas(), pickInfo, inputValue.get<Vec2>());
	if (isPick)
	{
		std::array<Vec2, 4> points = GetObb(pickInfo.currentSelectedPaint);
		mContext.hover = mContext.tempScene->createObb(points);
		mContext.hover.getComponent<StrokeComponent>().color = Style::HoverOutlineColor;
	}

	return true;
}
bool PickMode::onInputDetach(const InputValue& inputValue)
{
	mContext.isLeftMouseDown = false;
	return false;
}
}	 // namespace core