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
	shape.shape = tvg::Shape::gen();
	shape.shape->id = id.id;
	shape.shape->ref();
	mTvgScene->push(shape.shape);

	return entity;
}
Entity Scene::createRectFillStrokeLayer(std::string_view name, Vec2 xy, Vec2 wh)
{
	Entity entity = createRectFillLayer(name, xy, wh);
	auto& stroke = entity.addComponent<StrokeComponent>();
	return entity;
}

Entity Scene::createObb(const std::array<Vec2, 4>& points)
{
	auto entity = CreateEntity(this, "oob");
	auto& transform = entity.getComponent<TransformComponent>();
	auto& id = entity.getComponent<IDComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& path = entity.addComponent<PathComponent>();
	auto& stroke = entity.addComponent<StrokeComponent>();
	stroke.color = {255.0f, 127.0f, 63.0f};
	transform.position = points[0];

	auto bound = tvg::Shape::gen();
	path.pathCommands.push_back(tvg::PathCommand::MoveTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::Close);
	auto p1 = points[1] - points[0];
	auto p2 = points[2] - points[0];
	auto p3 = points[3] - points[0];
	path.points.emplace_back(0, 0);
	path.points.emplace_back(p1.x, p1.y);
	path.points.emplace_back(p2.x, p2.y);
	path.points.emplace_back(p3.x, p3.y);

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

void Scene::onUpdate()
{
	mRegistry.view<TransformComponent, PathComponent, ShapeComponent>().each([](auto entity, TransformComponent& transform, PathComponent& path, ShapeComponent& shape) {
		transform.update();
		shape.shape->transform(transform.transform);
		shape.shape->reset();
		shape.shape->appendPath(&path.pathCommands[0], path.pathCommands.size(), &path.points[0], path.points.size());
	});
	mRegistry.view<TransformComponent, RectPathComponent, ShapeComponent>().each([](auto entity, TransformComponent& transform, RectPathComponent& rect, ShapeComponent& shape) {
		transform.update();
		shape.shape->transform(transform.transform);
		shape.shape->reset();
		shape.shape->appendRect(rect.position.x, rect.position.y, rect.scale.x, rect.scale.y, rect.radius, rect.radius);
	});
	mRegistry.view<ShapeComponent, SolidFillComponent>().each([](auto entity, ShapeComponent& shape, SolidFillComponent& fill) {
		shape.shape->fill(fill.color.x, fill.color.y, fill.color.z, fill.alpha);
		shape.shape->fillRule(fill.rule);
	});
	mRegistry.view<ShapeComponent, StrokeComponent>().each([](auto entity, ShapeComponent& shape, StrokeComponent& stroke) {
		shape.shape->strokeWidth(stroke.width);
		shape.shape->strokeFill(stroke.color.x, stroke.color.y, stroke.color.z, 255);
	});
	mRegistry.view<SceneComponent>().each([this](auto entity, SceneComponent& scene) {
		if (scene.scene->mId != this->mId)
		{
			scene.scene->onUpdate();
		}
	});
	updateCanvas();
}

}	 // namespace core