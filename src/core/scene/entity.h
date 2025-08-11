#ifndef _CORE_SCENE_NSENTITY_H_
#define _CORE_SCENE_NSENTITY_H_

#include "../common/common.h"
#include <vector>
#include <memory>


#include <entt/entt.hpp>

namespace core
{
class Scene;
class Entity
{
public:
	Entity() = default;
	Entity(Scene* scene);
	virtual ~Entity() = default;

	template <class T>
	T& getComponent();

	template <typename T, typename... Args>
	T& addComponent(Args&&... args);

	template <typename T>
	bool hasComponent();

	entt::entity mHandle{entt::null};
private:
	core::Scene* rScene{nullptr};
};

}	 // namespace core

#endif
