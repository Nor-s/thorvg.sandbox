#ifndef _CORE_CANVAS_SHAPE_UTIL_H_
#define _CORE_CANVAS_SHAPE_UTIL_H_

#include <thorvg.h>
#include <array>
#include "common/common.h"

#include "scene/scene.h"

namespace core
{

static std::array<Vec2, 4> GetObb(tvg::Paint* p)
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
static bool IsInner(const std::array<Vec2, 4>& q, Vec2 p)
{
	const bool s1 = cross(q[0] - q[1], p - q[1]) >= 0.0f;
	const bool s2 = cross(q[1] - q[2], p - q[2]) >= 0.0f;
	const bool s3 = cross(q[2] - q[3], p - q[3]) >= 0.0f;
	const bool s4 = cross(q[3] - q[0], p - q[0]) >= 0.0f;
	return (s1 == s2) && (s2 == s3) && (s3 == s4);
}
static bool IsInner(tvg::Paint* p, Vec2 point)
{
	auto obb = GetObb(p);
	return IsInner(obb, point);
}
static Vec2 GetCenter(const std::array<Vec2, 4>& q)
{
	return (q[0] + q[1] + q[2] + q[3])/ 4.0f;
}

struct PickInfo
{
    std::unordered_set<uint32_t> excludeIds;
    core::Scene* currentSelectedScene{nullptr};
    tvg::Paint* currentSelectedPaint{nullptr};
};

static bool Pick(PickInfo& pickInfo, const Vec2& point, tvg::Paint* paint, int depth)
{
	if (!paint || pickInfo.excludeIds.find(paint->id) != pickInfo.excludeIds.end())
        return false;

	const bool isScene = (paint->type() == tvg::Type::Scene);
	const bool isCurrentSelected = pickInfo.currentSelectedPaint == paint;

	if (IsInner(GetObb(paint), point))
	{
		if (isScene)
		{
			tvg::Scene* rawScene = static_cast<tvg::Scene*>(paint);
			core::Scene* scene = Scene::FindScene(rawScene);
			if (scene == nullptr)
				return false;

			pickInfo.currentSelectedScene = scene;
			if (depth == 0 || isCurrentSelected)
			{
				bool isPicked = false;
				for (auto* p : rawScene->paints())
				{
					isPicked |= Pick(pickInfo, point, p, depth + 1);
				}
				return isCurrentSelected || isPicked;
			}
		}
		pickInfo.currentSelectedPaint = paint;
		return true;
	}
	return false;
}

template <typename T>
static bool Pick(T* canvasOrScene, PickInfo& pickInfo, const Vec2& point)
{
	auto& paints = canvasOrScene->paints();
	bool isPicked = false;
	for (auto& paint : paints)
	{
		isPicked |= Pick(pickInfo, point, paint, 0);
	}
	return isPicked;
}
}

#endif