#include "entity.h"
#include "scene.h"

#include "component/components.h"

namespace core
{

Entity::Entity(Scene* scene, uint32_t id)
{
	rScene = scene;
	mHandle = (entt::entity) id;
}
Entity::Entity(Scene* scene)
{
	rScene = scene;
	mHandle = scene->mRegistry.create();
}
void Entity::moveByDelta(const Vec2& delta)
{
	auto& transform = getComponent<TransformComponent>();
	transform.localCenterPosition = transform.localCenterPosition + delta;
}
void Entity::hide()
{
	if (mHandle == entt::null || mIsHide == true || hasComponent<ShapeComponent>() == false)
		return;

	if(auto& shape = getComponent<ShapeComponent>(); shape.shape != nullptr)
	{
		shape.shape->visible(false);
		mIsHide = true;
	}
}
void Entity::show()
{
	if (mHandle == entt::null || mIsHide == false || hasComponent<ShapeComponent>() == false)
		return;

	if(auto& shape = getComponent<ShapeComponent>(); shape.shape != nullptr)
	{
		shape.shape->visible(true);
		mIsHide = false;
	}
}
void Entity::update()
{
    auto& transform = getComponent<TransformComponent>();
    transform.update();

    auto& shape = getComponent<ShapeComponent>();
    shape.shape->reset();
    shape.shape->transform(transform.transform);

    if (hasComponent<PathComponent>())
    {
        auto& path = getComponent<PathComponent>();
        if (!path.pathCommands.empty() && !path.points.empty())
        {
            shape.shape->appendPath(path.pathCommands.data(), path.pathCommands.size(),
                                    path.points.data(),        path.points.size());
        }
    }
    else if (hasComponent<ElipsePathComponent>())
    {
        auto& path = getComponent<ElipsePathComponent>();
        shape.shape->appendCircle(path.position.x, path.position.y,
                                  path.scale.x * 0.5f, path.scale.y * 0.5f);
    }
    else if (hasComponent<RectPathComponent>())
    {
        auto& path = getComponent<RectPathComponent>();
        const float x = path.position.x - path.scale.x * 0.5f;
        const float y = path.position.y - path.scale.y * 0.5f;
        shape.shape->appendRect(x, y, path.scale.x, path.scale.y, path.radius, path.radius);
    }

    if (hasComponent<SolidFillComponent>())
    {
        auto& fill = getComponent<SolidFillComponent>();
        shape.shape->fill(fill.color.x, fill.color.y, fill.color.z, fill.alpha);
        shape.shape->fillRule(fill.rule);
    }
    if (hasComponent<StrokeComponent>())
    {
        auto& stroke = getComponent<StrokeComponent>();
        shape.shape->strokeWidth(stroke.width);
        shape.shape->strokeFill(stroke.color.x, stroke.color.y, stroke.color.z, stroke.alpha);
    }
}

}	 // namespace core
