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
	transform.position = transform.position + delta;
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

}	 // namespace core
