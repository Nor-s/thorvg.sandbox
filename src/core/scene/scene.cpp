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

Entity Scene::createRectLayer(std::string_view name)
{
	auto entity = CreateEntity(this, name);
	auto& id = entity.getComponent<IDComponent>();
	auto& transform = entity.getComponent<TransformComponent>();
	auto& rect = entity.addComponent<RectComponent>();
	auto& shape = entity.addComponent<ShpaeComponent>();
	auto& solidFill = entity.addComponent<SolidFillComponent>();
	auto color = solidFill.color;
	rect.radius = 0.0;
	shape.shape = tvg::Shape::gen();
	shape.shape->appendRect(transform.position.x, transform.position.y, transform.scale.x, transform.scale.y);
	shape.shape->fillRule(solidFill.rule);
	shape.shape->fill(color.x, color.y, color.z, color.a);
    shape.shape->id = id.id;

	shape.shape->ref();
	mTvgScene->push(shape.shape);

    updateCanvas();

	return entity;
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