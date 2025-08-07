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
	RelationshipComponent(EntityID parent)
		: parentId(parent) {}
};

struct ShpaeComponent
{
	tvg::Shape* shape{nullptr};
};
// shape
struct RectComponent
{
	float radius{0.0};
};

// property
struct TransformComponent
{
	Vec2 anchorPoint{0.0f, 0.0f};
	Vec2 position{0.0f, 0.0f};
	Vec2 scale{100.0f, 100.0f};
	float rotation{};
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

}	 // namespace core

#endif