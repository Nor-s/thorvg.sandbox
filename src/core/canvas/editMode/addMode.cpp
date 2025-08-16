#include "addMode.h"
#include "../canvas.h"
#include "../animationCreatorCanvas.h"
#include "scene/scene.h"

namespace core
{
AddMode::AddMode(AnimationCreatorCanvas* canvas, EditModeType type)
{
    type = mType;
	rCanvas = canvas;
	mContext.tempScene = std::make_unique<core::Scene>();
	mContext.tempScene->pushCanvas(rCanvas);
	rCanvas->getCanvas()->push(mContext.tempScene->getScene());
}
AddMode::~AddMode()
{
	rCanvas->getCanvas()->remove(mContext.tempScene->getScene());
}
void AddMode::onUpdate()
{
	if(mContext.tempScene)
	{
		mContext.tempScene->onUpdate();
	}
}
bool AddMode::onStarClickLefttMouse(const InputValue& inputValue)
{
	mContext.startPoint = inputValue.get<Vec2>();
	return true;
}
bool AddMode::onDragLeftMouse(const InputValue& inputValue)
{
	auto endPoint = inputValue.get<Vec2>();
	auto startPoint = mContext.startPoint;
	LOG_INFO("onDragLeftMouse: start {} {}, end {} {}", startPoint.x, startPoint.y, endPoint.x, endPoint.y);
	auto start = Vec2{std::min(startPoint.x, endPoint.x), std::min(startPoint.y, endPoint.y)};
	auto end = Vec2{std::max(startPoint.x, endPoint.x), std::max(startPoint.y, endPoint.y)};

	if(mContext.newEntity.mHandle != entt::null)
		mContext.tempScene->destroyEntity(mContext.newEntity);

	mContext.newEntity = mContext.tempScene->createRectFillLayer("Rect", start, end - start);
	return true;
}
bool AddMode::onEndLeftMouse(const InputValue& inputValue)
{
	auto endPoint = inputValue.get<Vec2>();
	auto startPoint = mContext.startPoint;
	auto start = Vec2{std::min(startPoint.x, endPoint.x), std::min(startPoint.y, endPoint.y)};
	auto end = Vec2{std::max(startPoint.x, endPoint.x), std::max(startPoint.y, endPoint.y)};

	if(mContext.newEntity.mHandle != entt::null)
	{
		mContext.tempScene->destroyEntity(mContext.newEntity);
	}
	auto wh = end-start;
	float threshold = 20.0f;
	if(wh.w > threshold && wh.h > threshold)
	{
		rCanvas->mScene->createRectFillLayer("Rect", start, wh);
	}
	return true;
}
bool AddMode::onInputDetach(const InputValue& inputValue)
{
	return false;
}

}	 // namespace core