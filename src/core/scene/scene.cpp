#include "scene.h"
#include "entity.h"
#include "component/components.h"

#include "canvas/animationCreatorCanvas.h"

#include <thorvg.h>

#include <algorithm>

namespace core
{
std::unordered_map<tvg::Scene*, Scene*> Scene::gSceneMap;

Scene::Scene(bool isMainScene)
	: mIsMainScene(isMainScene)
{
	auto entity = Scene::CreateEntity(this, "Scene");
	auto& scene = entity.addComponent<SceneComponent>();
	mId = entity.getComponent<IDComponent>().id;
	mTvgScene = tvg::Scene::gen();
	mTvgScene->ref();
	mTvgScene->id = mId;
	scene.scene = this;
	gSceneMap[mTvgScene] = this;
}

Scene::~Scene()
{
	mRegistry.view<ShapeComponent>().each([](auto entity, auto& shape) { shape.shape->unref(); });
	mTvgScene->unref();
}

Entity Scene::CreateEntity(Scene* scene, std::string_view name)
{
	static uint32_t id = 1;

	Entity entity(scene);

	entity.addComponent<IDComponent>(id);
	entity.addComponent<TransformComponent>();
	entity.addComponent<NameComponent>(name.empty() ? "Entity" : name);
	entity.addComponent<RelationshipComponent>();
	scene->mEntityMap[id] = entity;
	id++;

	return entity;
}

Scene* Scene::FindScene(tvg::Scene* scene)
{
	return gSceneMap[scene];
}

Entity Scene::createEntity(std::string_view name)
{
	return CreateEntity(this, name);
}

Entity Scene::createRectFillLayer(std::string_view name, Vec2 xy, Vec2 wh)
{
	auto entity = CreateEntity(this, name);
	auto& id = entity.getComponent<IDComponent>();
	auto& transform = entity.getComponent<TransformComponent>();
	auto& rect = entity.addComponent<RectPathComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& solidFill = entity.addComponent<SolidFillComponent>();
	auto color = solidFill.color;

	transform.anchorPoint = {xy.x, xy.y};
	transform.position = transform.anchorPoint;
	rect.scale = wh;
	rect.radius = 0.0f;
	rect.position = {0.0f, 0.0f};

	// push shape
	transform.update();
	shape.shape = tvg::Shape::gen();
	shape.shape->appendRect(rect.position.x, rect.position.y, rect.scale.x, rect.scale.y);
	shape.shape->fillRule(solidFill.rule);
	shape.shape->fill(color.x, color.y, color.z, solidFill.alpha);
	shape.shape->transform(transform.transform);
	shape.shape->id = id.id;
	shape.shape->ref();
	mTvgScene->push(shape.shape);

	updateCanvas();

	return entity;
}
Entity Scene::createRectFillStrokeLayer(std::string_view name, Vec2 xy, Vec2 wh)
{
	Entity entity = createRectFillLayer(name, xy, wh);
	auto& stroke = entity.addComponent<StrokeComponent>();
	auto& shape = entity.getComponent<ShapeComponent>();
	shape.shape->strokeWidth(stroke.width);
	shape.shape->strokeFill(stroke.color.x, stroke.color.y, stroke.color.z, 255);
	updateCanvas();
	return entity;
}
Entity Scene::createBbox(Vec2 xy, Vec2 wh)
{
	auto entity = CreateEntity(this, "bbox");
	auto& id = entity.getComponent<IDComponent>();
	auto& transform = entity.getComponent<TransformComponent>();
	auto& rect = entity.addComponent<RectPathComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& stroke = entity.addComponent<StrokeComponent>();

	transform.anchorPoint = {xy.x, xy.y};
	transform.position = transform.anchorPoint;
	rect.scale = wh;
	rect.radius = 0.0f;
	rect.position = {0.0f, 0.0f};

	auto bound = tvg::Shape::gen();
	bound->moveTo(xy.x, xy.y);
	bound->lineTo(xy.x + wh.w, xy.y);
	bound->lineTo(xy.x + wh.w, xy.y + wh.h);
	bound->lineTo(xy.x, xy.y + wh.h);
	bound->close();
	bound->strokeWidth(stroke.width);
	bound->strokeFill(stroke.color.x, stroke.color.y, stroke.color.z, 255);
	shape.shape = bound;
	shape.shape->ref();
	mTvgScene->push(shape.shape);

	updateCanvas();
	return entity;
}

Entity Scene::createObb(const std::array<Vec2, 4>& points)
{
	auto entity = CreateEntity(this, "oob");
	auto& transform = entity.getComponent<TransformComponent>();
	auto& rect = entity.addComponent<RectPathComponent>();
	auto& id = entity.getComponent<IDComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& stroke = entity.addComponent<StrokeComponent>();
	stroke.color = {255.0f, 127.0f, 63.0f};

	auto bound = tvg::Shape::gen();
	bound->moveTo(points[0].x, points[0].y);
	bound->lineTo(points[1].x, points[1].y);
	bound->lineTo(points[2].x, points[2].y);
	bound->lineTo(points[3].x, points[3].y);
	bound->close();
	bound->strokeWidth(stroke.width);
	bound->strokeFill(stroke.color.r, stroke.color.g, stroke.color.b, 255);

	shape.shape = bound;
	shape.shape->ref();
	shape.shape->id = id.id;
	mTvgScene->push(shape.shape);

	updateCanvas();

	return entity;
}

Entity Scene::getEntityById(uint32_t id)
{
	auto it = mEntityMap.find(id);
	assert(it != mEntityMap.end());
	return it->second;
}
Entity Scene::tryGetEntityById(uint32_t id)
{
	auto it = mEntityMap.find(id);
	if (it == mEntityMap.end())
		return Entity();
	return it->second;
}

void Scene::destroyEntity(core::Entity& entity)
{
	LOG_INFO("Destroying entity: {}", entity.getComponent<IDComponent>().id);

	if (entity.hasComponent<ShapeComponent>())
	{
		auto& shape = entity.getComponent<ShapeComponent>();
		shape.shape->unref();
		mTvgScene->remove(shape.shape);
		updateCanvas();
	}
	mEntityMap.erase(entity.getComponent<IDComponent>().id);
	mRegistry.destroy(entity.mHandle);
	entity.mHandle = entt::null;
}
void Scene::pushCanvas(CanvasWrapper* canvas)
{
	rCanvasList.push_back(canvas);
}

void Scene::updateCanvas()
{
	std::for_each(rCanvasList.begin(), rCanvasList.end(), [](CanvasWrapper* canvas) { canvas->update(); });
}

}	 // namespace core