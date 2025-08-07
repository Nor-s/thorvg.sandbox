#include "entity.h"

namespace core
{

Entity::Entity(Scene* scene)
{
	rScene = scene;
	mHandle = scene->mRegistry.create();
}

}	 // namespace core
