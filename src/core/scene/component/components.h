#ifndef __CORE_ENTITY_COMPONENTS_H_
#define __CORE_ENTITY_COMPONENTS_H_

#include <string>
#include <string_view>
#include <memory>
#include <vector>

#include <thorvg.h>

#include "common/common.h"

namespace core
{

class GlGeometry;

using EntityID = int32_t;

struct NameComponent
{
	std::string name{};

	NameComponent(std::string_view name) : name(name)
	{
	}
};
struct IDComponent
{
	EntityID id;
};
struct RelationshipComponent
{
	EntityID parentId{-1};
	std::vector<EntityID> children;

	RelationshipComponent() = default;
	RelationshipComponent(EntityID parent) : parentId(parent)
	{
	}
};

struct ShpaeComponent
{
	tvg::Shape* shape{nullptr};
};
// shape
struct RectComponent
{
	float radius{0.0};
	Vec2 position{0.0f, 0.0f};
	Vec2 dimension{100.0f, 100.0f};
};

// property
struct TransformComponent
{
	Vec2 anchorPoint{0.0f, 0.0f};
	Vec2 position{0.0f, 0.0f};
	Vec2 scale{1.0f, 1.0f};
	float rotation{};
	tvg::Matrix transform;
	void update()
	{
		transform = identity();
		applyScale(&transform, scale);
		applyRotate(&transform, rotation);
		applyTranslate(&transform, position);
	}
	static inline void identity(tvg::Matrix* m)
	{
		m->e11 = 1.0f;
		m->e12 = 0.0f;
		m->e13 = 0.0f;
		m->e21 = 0.0f;
		m->e22 = 1.0f;
		m->e23 = 0.0f;
		m->e31 = 0.0f;
		m->e32 = 0.0f;
		m->e33 = 1.0f;
	}

	static inline constexpr const tvg::Matrix identity()
	{
		return {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	}
	static inline void applyTranslate(tvg::Matrix* m, const Vec2& p)
	{
		m->e13 += p.x;
		m->e23 += p.y;
	}
	static inline void applyScale(tvg::Matrix* m, const Vec2& p)
	{
		m->e11 *= p.x;
		m->e22 *= p.y;
	}

	void applyRotate(tvg::Matrix* m, float degree)
	{
		if (degree == 0.0f)
			return;

		auto radian = degree / 180.0f * float(M_PI);
		auto cosVal = cosf(radian);
		auto sinVal = sinf(radian);

		m->e12 = m->e11 * -sinVal;
		m->e11 *= cosVal;
		m->e21 = m->e22 * sinVal;
		m->e22 *= cosVal;
	}
};
struct SolidFillComponent
{
	Vec4 color{255, 0, 0, 255};
	tvg::FillRule rule{tvg::FillRule::NonZero};
};

// keyframe
struct ColorKeyframes
{
	std::vector<Vec3> color;
};
struct OpacityKeyframes
{
	std::vector<float> alpha;
};
struct AnchorKeyframes
{
	std::vector<Vec2> point;
};
struct PositionKeyframes
{
	std::vector<Vec2> position;
};
struct ScaleKeyframes
{
	std::vector<Vec2> scale;
};
struct RotationKeyframes
{
	std::vector<float> rotation;
};

// for rect
struct RectDimensionKeyframes
{
	std::vector<Vec2> dimension;
};
struct RectRadiusKeyframes
{
	std::vector<float> radius;
};
struct RectPositionKeyframes
{
	std::vector<Vec2> radius;
};

}	 // namespace core

#endif