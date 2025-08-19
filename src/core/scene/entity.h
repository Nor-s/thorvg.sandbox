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
	Entity(Scene* scene, uint32_t id);
	Entity(Scene* scene);
	virtual ~Entity() = default;

	void moveByDelta(const Vec2& delta);
	void hide();
	void show();
	bool isHidden() const { return mIsHide; }

	void update();

	template <class T>
	T& getComponent();

	template <typename T, typename... Args>
	T& addComponent(Args&&... args);

	template <typename T>
	bool hasComponent();

	entt::entity mHandle{entt::null};
private:
	core::Scene* rScene{nullptr};
	bool mIsHide{false};
};

}	 // namespace core

#endif
