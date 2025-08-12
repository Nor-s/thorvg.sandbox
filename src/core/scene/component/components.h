#ifndef __CORE_ENTITY_COMPONENTS_H_
#define __CORE_ENTITY_COMPONENTS_H_

#include "common/common.h"
#include "../entity.h"

#include <string>
#include <string_view>
#include <memory>
#include <vector>

#include <thorvg.h>


namespace core
{

class GlGeometry;
class Scene;

using EntityID = uint32_t;

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
	EntityID parentId{0};
	std::vector<EntityID> children;

	RelationshipComponent() = default;
	RelationshipComponent(EntityID parent) : parentId(parent)
	{
	}
};

struct ShapeComponent
{
	Entity owner;
	tvg::Shape* shape{nullptr};
};

template <typename T>
struct Keyframes
{
	bool isEnable;
	std::vector<T> frames;
};

using FloatKeyFrame = Keyframes<float>;
using VectorKeyFrame = Keyframes<Vec2>;
using ColorKeyFrame = Keyframes<Vec3>;

struct SceneComponent
{
	Scene* scene{nullptr};
};

// shape
struct RectPathComponent
{
	float radius{0.0};
	Vec2 position{0.0f, 0.0f};
	Vec2 scale{100.0f, 100.0f};
	FloatKeyFrame radiusKeyframes;
	VectorKeyFrame positionKeyframes;
	VectorKeyFrame scaleKeyframes;
};

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
struct TransformKeyframeComponent
{
	VectorKeyFrame positionKeyframes;
	VectorKeyFrame scaleKeyframes;
	FloatKeyFrame rotationKeyframes;
};
struct SolidFillComponent
{
	Vec3 color{63.0f, 63.0f, 255.0f};
	float alpha{255.0f};
	tvg::FillRule rule{tvg::FillRule::NonZero};
	ColorKeyFrame colorKeyframe;
	FloatKeyFrame alphaKeyframe;
};

struct StrokeComponent
{
	Vec3 color{63.0f, 127.0f, 255.0f};
	float width{3.0f};
	// tvg::StrokeJoin join;
	ColorKeyFrame colorKeyframe;
	FloatKeyFrame widthKeyframe;
};

}	 // namespace core

#endif