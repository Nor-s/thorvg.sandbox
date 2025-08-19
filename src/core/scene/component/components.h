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

struct ElipsePathComponent
{
	Vec2 position{0.0f, 0.0f};
	Vec2 scale{100.0f, 100.0f};
	VectorKeyFrame positionKeyframes;
	VectorKeyFrame scaleKeyframes;
};

struct PathComponent
{
	std::vector<tvg::PathCommand> pathCommands;
	std::vector<tvg::Point> points;
	tvg::Point center;
};

struct TransformComponent
{
	Vec2 anchorPoint{0.0f, 0.0f};			 // local anchor
	Vec2 worldPosition{0.0f, 0.0f};			 // world position
	Vec2 localCenterPosition{0.0f, 0.0f};	 // center of layer
	Vec2 scale{1.0f, 1.0f};
	float rotation{};
	tvg::Matrix transform;

	// todo: parent transform
	void update()
	{
		transform = identity();
		applyTranslate(&transform, localCenterPosition);
		applyRotateR(&transform, rotation);
		applyScaleR(&transform, scale);
		applyTranslateR(&transform, anchorPoint * -1.0f);
		worldPosition = localCenterPosition + anchorPoint;
	}
	tvg::Matrix inverse() const
	{
		auto ret = identity();
		return inverse(&transform, &ret) ? ret : identity();
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
	static inline void applyScaleR(tvg::Matrix* m, const Vec2& p)
	{
		m->e11 *= p.x;
		m->e21 *= p.x;
		m->e12 *= p.y;
		m->e22 *= p.y;
	}
	static inline void applyTranslateR(tvg::Matrix* m, const Vec2& p)
	{
		if (p.x == 0.0f && p.y == 0.0f)
			return;
		m->e13 += (p.x * m->e11 + p.y * m->e12);
		m->e23 += (p.x * m->e21 + p.y * m->e22);
	}
	static void applyRotateR(tvg::Matrix* m, float degree)
	{
		if (degree == 0.0f)
			return;

		const float rad = degree * float(M_PI) / 180.0f;
		const float c = cosf(rad);
		const float s = sinf(rad);

		const float e11 = m->e11, e12 = m->e12, e13 = m->e13;
		const float e21 = m->e21, e22 = m->e22, e23 = m->e23;

		m->e11 = c * e11 + s * e12;
		m->e21 = c * e21 + s * e22;
		m->e12 = -s * e11 + c * e12;
		m->e22 = -s * e21 + c * e22;
	}
	
	static bool inverse(const tvg::Matrix* m, tvg::Matrix* out)
	{
		auto det = m->e11 * (m->e22 * m->e33 - m->e32 * m->e23) - m->e12 * (m->e21 * m->e33 - m->e23 * m->e31) +
				   m->e13 * (m->e21 * m->e32 - m->e22 * m->e31);

		auto invDet = 1.0f / det;
		if (std::isinf(invDet))
			return false;

		out->e11 = (m->e22 * m->e33 - m->e32 * m->e23) * invDet;
		out->e12 = (m->e13 * m->e32 - m->e12 * m->e33) * invDet;
		out->e13 = (m->e12 * m->e23 - m->e13 * m->e22) * invDet;
		out->e21 = (m->e23 * m->e31 - m->e21 * m->e33) * invDet;
		out->e22 = (m->e11 * m->e33 - m->e13 * m->e31) * invDet;
		out->e23 = (m->e21 * m->e13 - m->e11 * m->e23) * invDet;
		out->e31 = (m->e21 * m->e32 - m->e31 * m->e22) * invDet;
		out->e32 = (m->e31 * m->e12 - m->e11 * m->e32) * invDet;
		out->e33 = (m->e11 * m->e22 - m->e21 * m->e12) * invDet;

		return true;
	}
};
inline static void operator*=(Vec2& pt, const tvg::Matrix& m)
{
    auto tx = pt.x * m.e11 + pt.y * m.e12 + m.e13;
    auto ty = pt.x * m.e21 + pt.y * m.e22 + m.e23;
    pt.x = tx;
    pt.y = ty;
}


inline static Vec2 operator*(const Vec2& pt, const tvg::Matrix& m)
{
    auto tx = pt.x * m.e11 + pt.y * m.e12 + m.e13;
    auto ty = pt.x * m.e21 + pt.y * m.e22 + m.e23;
    return {tx, ty};
}

struct TransformKeyframeComponent
{
	VectorKeyFrame positionKeyframes;
	VectorKeyFrame scaleKeyframes;
	FloatKeyFrame rotationKeyframes;
};
struct SolidFillComponent
{
	Vec3 color = Style::DefaultFillColor;
	float alpha{255.0f};
	tvg::FillRule rule{tvg::FillRule::NonZero};
	ColorKeyFrame colorKeyframe;
	FloatKeyFrame alphaKeyframe;
};

struct StrokeComponent
{
	Vec3 color = Style::DefaultStrokeColor;
	float alpha{255.0f};
	float width{3.0f};
	// tvg::StrokeJoin join;
	ColorKeyFrame colorKeyframe;
	FloatKeyFrame widthKeyframe;
};

}	 // namespace core

#endif