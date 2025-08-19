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

Entity Scene::createEllipseFillLayer(std::string_view name, Vec2 minXy, Vec2 wh)
{
	auto entity = CreateEntity(this, name);
	auto& id = entity.getComponent<IDComponent>();
	auto& transform = entity.getComponent<TransformComponent>();
	auto& rect = entity.addComponent<ElipsePathComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& solidFill = entity.addComponent<SolidFillComponent>();

	transform.localCenterPosition = minXy + wh *0.5f;
	transform.anchorPoint = {0.0f, 0.0f}; 
	rect.scale = wh;
	rect.position = {0.0f, 0.0f};

	// push shape
	shape.shape = tvg::Shape::gen();
	shape.shape->id = id.id;
	shape.shape->ref();

	entity.update();
	mTvgScene->push(shape.shape);

	return entity;
}

Entity Scene::createEllipseFillStrokeLayer(std::string_view name, Vec2 minXy, Vec2 wh)
{
	Entity entity = createEllipseFillLayer(name, minXy, wh);
	auto& stroke = entity.addComponent<StrokeComponent>();
	entity.update();
	return entity;
}

Entity Scene::createRectFillLayer(std::string_view name, Vec2 minXy, Vec2 wh)
{
	auto entity = CreateEntity(this, name);
	auto& id = entity.getComponent<IDComponent>();
	auto& transform = entity.getComponent<TransformComponent>();
	auto& rect = entity.addComponent<RectPathComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& solidFill = entity.addComponent<SolidFillComponent>();

	transform.localCenterPosition = minXy + wh *0.5f;
	transform.anchorPoint = {0.0f, 0.0f}; 
	rect.scale = wh;
	rect.radius = 0.0f;
	rect.position = {0.0f, 0.0f};

	// push shape
	shape.shape = tvg::Shape::gen();
	shape.shape->id = id.id;
	shape.shape->ref();

	entity.update();
	mTvgScene->push(shape.shape);

	return entity;
}
Entity Scene::createRectFillStrokeLayer(std::string_view name, Vec2 minXy, Vec2 wh)
{
	Entity entity = createRectFillLayer(name, minXy, wh);
	auto& stroke = entity.addComponent<StrokeComponent>();
	entity.update();
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
	auto minx = std::min({points[0].x, points[1].x, points[2].x, points[3].x});
	auto maxx = std::max({points[0].x, points[1].x, points[2].x, points[3].x});
	auto miny = std::min({points[0].y, points[1].y, points[2].y, points[3].y});
	auto maxy = std::max({points[0].y, points[1].y, points[2].y, points[3].y});
	auto width = maxx - minx;
	auto height = maxy - miny;
	transform.anchorPoint = {0.0f, 0.0f}; // oring of the local, center of the bbox
	transform.localCenterPosition = {minx + width*0.5f, miny + height*0.5f};

	auto bound = tvg::Shape::gen();
	path.pathCommands.push_back(tvg::PathCommand::MoveTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::Close);
	path.center = tvg::Point{width/2, height/2};
	auto centerp = transform.localCenterPosition;
	auto p0 = points[0] - centerp;
	auto p1 = points[1] - centerp;
	auto p2 = points[2] - centerp;
	auto p3 = points[3] - centerp;
	path.points.emplace_back(p0.x, p0.y);
	path.points.emplace_back(p1.x, p1.y);
	path.points.emplace_back(p2.x, p2.y);
	path.points.emplace_back(p3.x, p3.y);

	shape.shape = bound;
	shape.shape->ref();
	shape.shape->id = id.id;

	entity.update();
	mTvgScene->push(shape.shape);

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
	// LOG_INFO("Destroying entity: {}", entity.getComponent<IDComponent>().id);
	if (entity.hasComponent<ShapeComponent>())
	{
		auto& shape = entity.getComponent<ShapeComponent>();
		mTvgScene->remove(shape.shape);
		shape.shape->unref();
	}
	mEntityMap.erase(entity.getComponent<IDComponent>().id);
	mRegistry.destroy(entity.mHandle);
	entity.mHandle = entt::null;
}
void Scene::pushCanvas(CanvasWrapper* canvas)
{
	rCanvasList.push_back(canvas);
}

void Scene::onUpdate()
{
	mRegistry.view<TransformComponent, PathComponent, ShapeComponent>().each([](auto entity, TransformComponent& transform, PathComponent& path, ShapeComponent& shape) {
		transform.update();
		shape.shape->reset();
		shape.shape->transform(transform.transform);
		auto pathPoint = path.points;
		// todo: path update
		shape.shape->appendPath(&path.pathCommands[0], path.pathCommands.size(), &pathPoint[0], path.points.size());
	});
	mRegistry.view<TransformComponent, ElipsePathComponent, ShapeComponent>().each([](auto entity, TransformComponent& transform, ElipsePathComponent& path, ShapeComponent& shape) {
		transform.update();
		shape.shape->reset();
		shape.shape->transform(transform.transform);
		shape.shape->appendCircle(path.position.x, path.position.y, path.scale.x*0.5f, path.scale.y*0.5f);
	});
	mRegistry.view<TransformComponent, RectPathComponent, ShapeComponent>().each([](auto entity, TransformComponent& transform, RectPathComponent& path, ShapeComponent& shape) {
		transform.update();
		shape.shape->reset();
		shape.shape->transform(transform.transform);
		shape.shape->appendRect(path.position.x - path.scale.x/2.0f, path.position.y - path.scale.y/2.0f, path.scale.x, path.scale.y, path.radius, path.radius);
	});
	mRegistry.view<ShapeComponent, SolidFillComponent>().each([](auto entity, ShapeComponent& shape, SolidFillComponent& fill) {
		shape.shape->fill(fill.color.x, fill.color.y, fill.color.z, fill.alpha);
		shape.shape->fillRule(fill.rule);
	});
	mRegistry.view<ShapeComponent, StrokeComponent>().each([](auto entity, ShapeComponent& shape, StrokeComponent& stroke) {
		shape.shape->strokeWidth(stroke.width);
		shape.shape->strokeFill(stroke.color.x, stroke.color.y, stroke.color.z, stroke.alpha);
	});
	mRegistry.view<SceneComponent>().each([this](auto entity, SceneComponent& scene) {
		if (scene.scene->mId != this->mId)
		{
			scene.scene->onUpdate();
		}
	});
}

}	 // namespace core