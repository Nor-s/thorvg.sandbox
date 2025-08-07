#ifndef _CORE_SCENE_NSENTITY_H_
#define _CORE_SCENE_NSENTITY_H_

#include "../common/common.h"
#include <vector>
#include <memory>

#include "scene.h"

#include <entt/entt.hpp>

namespace core
{

class Entity
{
public:
	Entity() = default;
	Entity(Scene* scene);
	virtual ~Entity() = default;

	template <class T>
	T& getComponent()
	{
		assert(hasComponent<T>());
		return rScene->mRegistry.get<T>(mHandle);
	}

	template <typename T, typename... Args>
	T& addComponent(Args&&... args)
	{
		assert(!hasComponent<T>());
		return rScene->mRegistry.emplace<T>(mHandle, std::forward<Args>(args)...);
	}

	template <typename T>
	bool hasComponent()
	{
		assert(mHandle != entt::null);
		return rScene->mRegistry.try_get<T>(mHandle) != nullptr;
	}

private:
	entt::entity mHandle{entt::null};
	core::Scene* rScene{nullptr};
};

}	 // namespace core

#endif
