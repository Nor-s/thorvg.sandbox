#include "scene.h"
#include "entity.h"
#include "component/components.h"

#include "canvas/animationCreatorCanvas.h"

#include <thorvg.h>

#include <algorithm>

namespace core
{
Scene::Scene()
{
	mTvgScene = tvg::Scene::gen();
	mTvgScene->ref();
}

Scene::~Scene()
{
	mTvgScene->unref();
}

Entity Scene::CreateEntity(Scene* scene, std::string_view name)
{
	static int id = 1;
	Entity entity(scene);
	entity.addComponent<IDComponent>(id++);
	entity.addComponent<TransformComponent>();
	entity.addComponent<NameComponent>(name.empty() ? "Entity" : name);
	entity.addComponent<RelationshipComponent>();

	return entity;
}

Entity Scene::createEntity(std::string_view name)
{
	return CreateEntity(this, name);
}

Entity Scene::createRectLayer(std::string_view name, Vec2 xy, Vec2 wh)
{
	auto entity = CreateEntity(this, name);
	auto& id = entity.getComponent<IDComponent>();
	auto& transform = entity.getComponent<TransformComponent>();
	auto& rect = entity.addComponent<RectPathComponent>();
	auto& shape = entity.addComponent<ShpaeComponent>();
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
void Scene::destroyEntity(core::Entity& entity)
{
	if(entity.hasComponent<ShpaeComponent>())
	{
		auto& shape = entity.getComponent<ShpaeComponent>();
		shape.shape->unref();
		mTvgScene->remove(shape.shape);
		updateCanvas();
	}

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