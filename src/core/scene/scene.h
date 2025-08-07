#ifndef _CORE_SCENE_SCENE_H_
#define _CORE_SCENE_SCENE_H_

#include <entt/entt.hpp>
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
	Entity createRectLayer(std::string_view name);

	void pushCanvas(CanvasWrapper* canvas);
	void updateCanvas();

protected:
	friend class Entity;
	friend class AnimationCreatorCanvas;
	entt::registry mRegistry{};
	std::vector<CanvasWrapper*> rCanvasList;
	tvg::Scene* mTvgScene;
};

}	 // namespace core

#endif
