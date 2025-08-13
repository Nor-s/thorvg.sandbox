#include "pickMode.h"
#include "../canvas.h"
#include "../animationCreatorCanvas.h"
#include "scene/scene.h"
#include "scene/component/components.h"


#include <thorvg.h>
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
std::array<Vec2, 4> GetObb(tvg::Paint* p)
{
	std::array<tvg::Point, 4> pts;
	std::array<Vec2, 4> ptsVec;
	p->bounds(pts.data());
	ptsVec[0] = Vec2{pts[0].x, pts[0].y};
	ptsVec[1] = Vec2{pts[1].x, pts[1].y};
	ptsVec[2] = Vec2{pts[2].x, pts[2].y};
	ptsVec[3] = Vec2{pts[3].x, pts[3].y};
	return ptsVec;
}
bool IsInner(const std::array<Vec2, 4>& q, Vec2 p)
{
	const bool s1 = cross(q[0] - q[1], p - q[1]) >= 0.0f;
	const bool s2 = cross(q[1] - q[2], p - q[2]) >= 0.0f;
	const bool s3 = cross(q[2] - q[3], p - q[3]) >= 0.0f;
	const bool s4 = cross(q[3] - q[0], p - q[0]) >= 0.0f;
	return (s1 == s2) && (s2 == s3) && (s3 == s4);
}

bool PickMode::pick(const InputValue& inputValue, tvg::Paint* paint, int depth)
{
	if (!paint)
		return false;

	const bool isScene = (paint->type() == tvg::Type::Scene);
	const Vec2 point = inputValue.get<Vec2>();
	const bool isCurrentSelected = mContext.currentSelectedPaint == paint;

	if (IsInner(GetObb(paint), point))
	{
		if (isScene)
		{
			tvg::Scene* rawScene = static_cast<tvg::Scene*>(paint);
			core::Scene* scene = Scene::FindScene(rawScene);
			if (scene == nullptr)
				return false;

			mContext.currentSelectedScene = scene;
			if (depth == 0 || isCurrentSelected)
			{
				bool isPicked = false;
				for (auto* p : rawScene->paints())
				{
					isPicked |= pick(inputValue, p, depth + 1);
				}
				return isCurrentSelected || isPicked;
			}
		}
		mContext.currentSelectedPaint = paint;
		return true;
	}
	return false;
}

void PickMode::onStarClickLefttMouse(const InputValue& inputValue)
{
	mContext.isLeftMouseDown = true;
	mContext.startPoint = inputValue.get<Vec2>();
	mContext.beforePoint = mContext.startPoint;
	// pick process
	// is control point?
	auto& paints = rCanvas->getCanvas()->paints();
	bool isPicked = false;
	for (auto& paint : paints)
	{
		if(paint->id == mContext.tempScene->mId) continue;
		isPicked |= pick(inputValue, paint, 0);
	}
	if(mContext.bbox.mHandle != entt::null)
			mContext.tempScene->destroyEntity(mContext.bbox);
	if(isPicked && mContext.currentSelectedScene)
	{
		std::array<Vec2, 4> points = GetObb(mContext.currentSelectedPaint);
		mContext.bbox = mContext.tempScene->createObb(points);
	}
	else 
	{
		mContext.currentSelectedPaint = nullptr;
		mContext.currentSelectedScene = nullptr;
	}
}
void PickMode::onDragLeftMouse(const InputValue& inputValue)
{
	if(mContext.isLeftMouseDown == false) return;

	auto endPoint = inputValue.get<Vec2>();
	auto startPoint = mContext.startPoint;
	auto start = Vec2{std::min(startPoint.x, endPoint.x), std::min(startPoint.y, endPoint.y)};
	auto end = Vec2{std::max(startPoint.x, endPoint.x), std::max(startPoint.y, endPoint.y)};
	auto delta = endPoint - mContext.beforePoint;
	mContext.beforePoint = endPoint;

	if (mContext.drag.mHandle != entt::null)
		mContext.tempScene->destroyEntity(mContext.drag);

	if(mContext.currentSelectedPaint)
	{
		auto id = mContext.currentSelectedPaint->id;
		auto entity = mContext.currentSelectedScene->getEntityById(id);
		entity.moveByDelta(delta);
		mContext.bbox.moveByDelta(delta);
		return;
	}

	// drag process
	mContext.drag = mContext.tempScene->createRectFillStrokeLayer("drag", start, end - start);
	auto& shape = mContext.drag.getComponent<ShapeComponent>();
	auto& fill = mContext.drag.getComponent<SolidFillComponent>();
	fill.alpha *= 0.3f;
	shape.shape->fill(fill.color.x, fill.color.y, fill.color.z, fill.alpha);
	mContext.tempScene->updateCanvas();
}
void PickMode::onEndLeftMouse(const InputValue& inputValue)
{
	if(mContext.isLeftMouseDown == false) return;
	if (mContext.drag.mHandle != entt::null)
		mContext.tempScene->destroyEntity(mContext.drag);
	mContext.isLeftMouseDown = false;
}
void PickMode::onInputDetach(const InputValue& inputValue)
{
	mContext.isLeftMouseDown = false;
}
}	 // namespace core