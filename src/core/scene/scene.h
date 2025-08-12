#ifndef _CORE_SCENE_SCENE_H_
#define _CORE_SCENE_SCENE_H_

#include "entity.h"

#include "common/common.h"

#include <thorvg.h>
#include <unordered_map>
#include <array>


namespace core
{

class CanvasWrapper;
class Entity;

class Scene
{
public:
	static Entity CreateEntity(Scene* scene, std::string_view name);
	static Scene* FindScene(tvg::Scene* scene);

protected:
	static std::unordered_map<tvg::Scene*, Scene*> gSceneMap;

public:
	Scene(bool isMainScene = false);
	~Scene();
	entt::registry& getRegistry()
	{
		return mRegistry;
	}
	Entity createEntity(std::string_view name);
	Entity createRectFillLayer(std::string_view name, Vec2 xy, Vec2 wh);
	Entity createRectFillStrokeLayer(std::string_view name, Vec2 xy, Vec2 wh);
	Entity createBbox(Vec2 xy, Vec2 wh);
	Entity createObb(const std::array<Vec2, 4>& points);

	Entity getEntityById(uint32_t id);
	Entity tryGetEntityById(uint32_t id);

	void destroyEntity(core::Entity& entity);
	void pushCanvas(CanvasWrapper* canvas);
	void updateCanvas();

	tvg::Scene* getScene() 
	{
		return mTvgScene;
	}

	uint32_t mId;
	const bool mIsMainScene;

protected:
	friend class Entity;
	friend class AnimationCreatorCanvas;
	entt::registry mRegistry{};
	std::vector<CanvasWrapper*> rCanvasList;
	tvg::Scene* mTvgScene;

	std::unordered_map<uint32_t, Entity> mEntityMap;
};

}	 // namespace core

#include "entity.hpp"

#endif
