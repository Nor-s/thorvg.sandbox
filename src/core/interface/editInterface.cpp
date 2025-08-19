#include "editInterface.h"

#include "scene/scene.h"
#include "scene/component/components.h"

using namespace core;

namespace helper
{

Scene* FindScene(SCENE_ID id)
{
	auto entity = core::Scene::FindEntity(id);

	if (entity.mHandle != entt::null && entity.hasComponent<SceneComponent>())
	{
		auto& scene = entity.getComponent<SceneComponent>();

		if (scene.scene == nullptr)
			return nullptr;
		return scene.scene;
	}

	return nullptr;
}

}	 // namespace helper

EDIT_API ENTITY_ID CreateRectPathEntity(SCENE_ID id, float minX, float minY, float w, float h)
{
	auto* scene = helper::FindScene(id);
	if (scene == nullptr)
		return 0;

	auto entity = scene->createRectFillLayer(Vec2{minX, minY}, Vec2{w, h});
	return entity.getComponent<IDComponent>().id;
}

EDIT_API ENTITY_ID CreateElipsePathEntity(SCENE_ID id, float minX, float minY, float w, float h)
{
	auto* scene = helper::FindScene(id);
	if (scene == nullptr)
		return 0;

	auto entity = scene->createEllipseFillLayer(Vec2{minX, minY}, Vec2{w, h});
	return entity.getComponent<IDComponent>().id;
}

EDIT_API Edit_Result UpdateEntityTransformCurrentFrame(ENTITY_ID id, UpdateEntityTransform* transform, bool isEnd)
{
	auto entity = Scene::FindEntity(id);
    if(entity.isNull()) return EDIT_RESULT_INVALID_ENTITY;

    LOG_CRITICAL("TODO: UpdateEntityTransformCurrentFrame");

	return EDIT_RESULT_SUCCESS;
}

EDIT_API Edit_Result UpdateEntityRotationCurrentFrame(ENTITY_ID id, float x, bool isEnd)
{
	auto entity = Scene::FindEntity(id);
    if(entity.isNull()) return EDIT_RESULT_INVALID_ENTITY;

    LOG_CRITICAL("TODO: UpdateEntityRotationCurrentFrame");

	return EDIT_RESULT_SUCCESS;
}

EDIT_API Edit_Result UpdateEntityPositionCurrentFrame(ENTITY_ID id, float x, float y, bool isEnd)
{
	auto entity = Scene::FindEntity(id);
    if(entity.isNull()) return EDIT_RESULT_INVALID_ENTITY;

    LOG_CRITICAL("TODO: UpdateEntityPositionCurrentFrame");

	return EDIT_RESULT_SUCCESS;
}

EDIT_API Edit_Result UpdateEntityScaleCurrentFrame(ENTITY_ID id, float x, float y, bool isEnd)
{
	auto entity = Scene::FindEntity(id);
    if(entity.isNull()) return EDIT_RESULT_INVALID_ENTITY;

    auto& tr = entity.getComponent<TransformComponent>();
    tr.scale.x = x;
    tr.scale.y = y;

	return EDIT_RESULT_SUCCESS;
}

EDIT_API Edit_Result UpdateEntityDeltaRotationCurrentFrame(ENTITY_ID id, float x, bool isEnd)
{
	auto entity = Scene::FindEntity(id);
    if(entity.isNull()) return EDIT_RESULT_INVALID_ENTITY;

    auto& tr = entity.getComponent<TransformComponent>();
    tr.rotation += x;

	return EDIT_RESULT_SUCCESS;
}

EDIT_API Edit_Result UpdateEntityDeltaPositionCurrentFrame(ENTITY_ID id, float x, float y, bool isEnd)
{
	auto entity = Scene::FindEntity(id);
    if(entity.isNull()) return EDIT_RESULT_INVALID_ENTITY;

    entity.moveByDelta({x, y});

	return EDIT_RESULT_SUCCESS;
}


EDIT_API Edit_Result UpdateEntityEnd(ENTITY_ID id)
{
	return EDIT_RESULT_SUCCESS;
}
