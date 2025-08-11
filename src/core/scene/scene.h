#ifndef _CORE_SCENE_SCENE_H_
#define _CORE_SCENE_SCENE_H_

#include "entity.h"

#include "common/common.h"
#include <thorvg.h>


namespace core
{

class CanvasWrapper;
class Entity;

class Scene
{
public:
	static Entity CreateEntity(Scene* scene, std::string_view name);

public:
	Scene();
	~Scene();
	entt::registry& getRegistry()
	{
		return mRegistry;
	}
	Entity createEntity(std::string_view name);
	Entity createRectLayer(std::string_view name, Vec2 xy, Vec2 wh);
	void destroyEntity(core::Entity& entity);
	void pushCanvas(CanvasWrapper* canvas);
	void updateCanvas();

	tvg::Scene* getScene() 
	{
		return mTvgScene;
	}

protected:
	friend class Entity;
	friend class AnimationCreatorCanvas;
	entt::registry mRegistry{};
	std::vector<CanvasWrapper*> rCanvasList;
	tvg::Scene* mTvgScene;
};

}	 // namespace core

#include "entity.hpp"

#endif
