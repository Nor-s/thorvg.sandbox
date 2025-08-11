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
void AddMode::onStarClickLefttMouse(const InputValue& inputValue)
{
	mContext.startPoint = inputValue.get<Vec2>();
}
void AddMode::onDragLeftMouse(const InputValue& inputValue)
{
	auto endPoint = inputValue.get<Vec2>();
	auto startPoint = mContext.startPoint;
	auto start = Vec2{std::min(startPoint.x, endPoint.x), std::min(startPoint.y, endPoint.y)};
	auto end = Vec2{std::max(startPoint.x, endPoint.x), std::max(startPoint.y, endPoint.y)};

	if(mContext.newEntity.mHandle != entt::null)
		mContext.tempScene->destroyEntity(mContext.newEntity);

	mContext.newEntity = mContext.tempScene->createRectLayer("Rect", start, end - start);
}
void AddMode::onEndLeftMouse(const InputValue& inputValue)
{
	auto endPoint = inputValue.get<Vec2>();
	auto startPoint = mContext.startPoint;
	auto start = Vec2{std::min(startPoint.x, endPoint.x), std::min(startPoint.y, endPoint.y)};
	auto end = Vec2{std::max(startPoint.x, endPoint.x), std::max(startPoint.y, endPoint.y)};

	if(mContext.newEntity.mHandle != entt::null)
	{
		mContext.tempScene->destroyEntity(mContext.newEntity);
	}
	rCanvas->mScene->createRectLayer("Rect", start, end - start);
}
void AddMode::onInputDetach(const InputValue& inputValue)
{
}

}	 // namespace core