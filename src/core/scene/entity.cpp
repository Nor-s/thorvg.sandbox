#include "entity.h"
#include "scene.h"

#include "component/components.h"

namespace core
{

Entity::Entity(Scene* scene, uint32_t id)
{
	rScene = scene;
	mHandle = (entt::entity)id;
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

}	 // namespace core
