#include "pickMode.h"
#include "../canvas.h"
#include "../animationCreatorCanvas.h"
#include "scene/scene.h"
#include "scene/component/components.h"

namespace core
{

PickMode::PickMode(AnimationCreatorCanvas* canvas)
{
	rCanvas = canvas;
	mContext.tempScene = std::make_unique<core::Scene>();
	mContext.tempScene->pushCanvas(rCanvas);
	rCanvas->getCanvas()->push(mContext.tempScene->getScene());
}
PickMode::~PickMode()
{
	rCanvas->getCanvas()->remove(mContext.tempScene->getScene());
}

void PickMode::onStarClickLefttMouse(const InputValue& inputValue)
{
	mContext.startPoint = inputValue.get<Vec2>();
}
void PickMode::onDragLeftMouse(const InputValue& inputValue)
{
	auto endPoint = inputValue.get<Vec2>();
	auto startPoint = mContext.startPoint;
	auto start = Vec2{std::min(startPoint.x, endPoint.x), std::min(startPoint.y, endPoint.y)};
	auto end = Vec2{std::max(startPoint.x, endPoint.x), std::max(startPoint.y, endPoint.y)};

	if(mContext.drag.mHandle != entt::null)
		mContext.tempScene->destroyEntity(mContext.drag);

	mContext.drag = mContext.tempScene->createRectFillStrokeLayer("drag",start, end - start);
	auto& shape = mContext.drag.getComponent<ShapeComponent>();
	auto& fill = mContext.drag.getComponent<SolidFillComponent>();
	fill.alpha *= 0.3f;
	shape.shape->fill(fill.color.x, fill.color.y, fill.color.z, fill.alpha);
	mContext.tempScene->updateCanvas();
}
void PickMode::onEndLeftMouse(const InputValue& inputValue)
{
	if(mContext.drag.mHandle != entt::null)
		mContext.tempScene->destroyEntity(mContext.drag);
}
void PickMode::onInputDetach(const InputValue& inputValue)
{
}
}	 // namespace core