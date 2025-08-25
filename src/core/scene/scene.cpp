#include "scene.h"
#include "entity.h"
#include "component/components.h"
#include "component/uiComponents.h"

#include "canvas/animationCreatorCanvas.h"
#include "interface/editInterface.h"
#include "animation/animator.h"

#include "ui/bbox.h"
#include "canvas/shapeUtil.h"

#include <thorvg.h>

#include <algorithm>

namespace core
{
std::unordered_map<tvg::Scene*, Scene*> Scene::gSceneMap;
std::unordered_map<uint32_t, Entity> Scene::gEntityMap;

Scene::Scene(bool isMainScene) : mIsMainScene(isMainScene)
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
	scene->gEntityMap[id] = entity;
	id++;

	return entity;
}

Scene* Scene::FindScene(tvg::Scene* scene)
{
	return gSceneMap[scene];
}

Entity Scene::FindEntity(uint32_t entityId)
{
	if (auto it = gEntityMap.find(entityId); it != gEntityMap.end())
	{
		return it->second;
	}
	return Entity();
}

Entity Scene::createEntity(std::string_view name)
{
	return CreateEntity(this, name);
}

Entity Scene::createEllipseFillLayer(Vec2 minXy, Vec2 wh)
{
	auto entity = CreateEntity(this, "Ellipse Layer");
	auto& id = entity.getComponent<IDComponent>();
	auto& transform = entity.getComponent<TransformComponent>();
	auto& rect = entity.addComponent<ElipsePathComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& solidFill = entity.addComponent<SolidFillComponent>();

	transform.localCenterPosition = minXy + wh * 0.5f;
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

Entity Scene::createEllipseFillStrokeLayer(Vec2 minXy, Vec2 wh)
{
	Entity entity = createEllipseFillLayer(minXy, wh);
	auto& stroke = entity.addComponent<StrokeComponent>();
	entity.update();
	return entity;
}

Entity Scene::createRectFillLayer(Vec2 minXy, Vec2 wh)
{
	auto entity = CreateEntity(this, "Rect Layer");
	auto& id = entity.getComponent<IDComponent>();
	auto& transform = entity.getComponent<TransformComponent>();
	auto& rect = entity.addComponent<RectPathComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& solidFill = entity.addComponent<SolidFillComponent>();

	transform.localCenterPosition = minXy + wh * 0.5f;
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
Entity Scene::createRectFillStrokeLayer(Vec2 minXy, Vec2 wh)
{
	Entity entity = createRectFillLayer(minXy, wh);
	auto& stroke = entity.addComponent<StrokeComponent>();
	entity.update();
	return entity;
}

Entity Scene::createPolygonFillLayer(Vec2 minXy, Vec2 wh)
{
	auto entity = CreateEntity(this, "polygon");
	auto& transform = entity.getComponent<TransformComponent>();
	auto& id = entity.getComponent<IDComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& path = entity.addComponent<PathComponent>();
	entity.addComponent<SolidFillComponent>();
	transform.anchorPoint = {0.0f, 0.0f};	 // oring of the local, center of the bbox
	transform.localCenterPosition = minXy + wh * 0.5f;

	auto bound = tvg::Shape::gen();
	path.pathCommands.push_back(tvg::PathCommand::MoveTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::Close);
	path.center = {transform.localCenterPosition.x, transform.localCenterPosition.y};

	auto p0 = Vec2{path.center.x, minXy.y} - transform.localCenterPosition;
	auto p1 = Vec2{minXy.x + wh.x, minXy.y + wh.h} - transform.localCenterPosition;
	auto p2 = Vec2{minXy.x, minXy.y + wh.h} - transform.localCenterPosition;

	path.points.emplace_back(p0.x, p0.y);
	path.points.emplace_back(p1.x, p1.y);
	path.points.emplace_back(p2.x, p2.y);

	shape.shape = bound;
	shape.shape->ref();
	shape.shape->id = id.id;

	entity.update();
	mTvgScene->push(shape.shape);

	return entity;
}

Entity Scene::createStarFillLayer(Vec2 minXy, Vec2 wh)
{
	if (wh.w*wh.h < 1e-6)
	{
		wh.w = wh.h = 1.0f;
	}
	auto entity = CreateEntity(this, "Star");
	auto& transform = entity.getComponent<TransformComponent>();
	auto& id = entity.getComponent<IDComponent>();
	auto& shape = entity.addComponent<ShapeComponent>();
	auto& path = entity.addComponent<PathComponent>();
	entity.addComponent<SolidFillComponent>();
	transform.anchorPoint = {0.0f, 0.0f};	 // oring of the local, center of the bbox
	transform.localCenterPosition = minXy + wh * 0.5f;
	shape.shape = tvg::Shape::gen();
	shape.shape->ref();
	shape.shape->id = id.id;

	const auto scale100HalfWidth = 50.0f;
	Vec2 ratio = wh*0.5f/scale100HalfWidth;
	Vec2 p[5];
	Vec2 o[5];

	path.center = {transform.localCenterPosition.x, transform.localCenterPosition.y};
	path.pathCommands.push_back(tvg::PathCommand::MoveTo);
	p[0] = Vec2{0.0f, -scale100HalfWidth};
	for (int i = 1; i < 5; i++)
	{
		p[i] = Rotate(p[i-1], 72);
		path.pathCommands.push_back(tvg::PathCommand::LineTo);
	}
	Line line = Line::Gen({0.0f, 0.0f}, -54.0f);
	o[0] = p[0];
	Intersect(line, Segment{.start=p[0], .end=p[2]}, o[0]);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	for (int i = 1; i < 5; i++)
	{
		o[i] = Rotate(o[i-1], 72);
		path.pathCommands.push_back(tvg::PathCommand::LineTo);
	}

	path.pathCommands.push_back(tvg::PathCommand::Close);

	for (int i = 0; i < 5; i++)
	{
		p[i] = {p[i].x * ratio.x, p[i].y * ratio.y};
		o[i] = {o[i].x * ratio.x, o[i].y * ratio.y};
		path.points.emplace_back(p[i].x, p[i].y);
		path.points.emplace_back(o[i].x, o[i].y);
	}
	entity.update();
	mTvgScene->push(shape.shape);

	return entity;
}

Entity Scene::createObb(const std::array<Vec2, 4>& points)
{
	auto entity = CreateEntity(this, "obb");
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
	transform.anchorPoint = {0.0f, 0.0f};	 // oring of the local, center of the bbox
	transform.localCenterPosition = {minx + width * 0.5f, miny + height * 0.5f};

	auto bound = tvg::Shape::gen();
	path.pathCommands.push_back(tvg::PathCommand::MoveTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::LineTo);
	path.pathCommands.push_back(tvg::PathCommand::Close);
	path.center = tvg::Point{width / 2, height / 2};
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
	auto it = gEntityMap.find(id);
	assert(it != gEntityMap.end());
	return it->second;
}
Entity Scene::tryGetEntityById(uint32_t id)
{
	auto it = gEntityMap.find(id);
	if (it == gEntityMap.end())
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
	gEntityMap.erase(entity.getComponent<IDComponent>().id);
	mRegistry.destroy(entity.mHandle);
	entity.mHandle = entt::null;
}
void Scene::pushCanvas(CanvasWrapper* canvas)
{
	rCanvasList.push_back(canvas);
}

void Scene::onUpdate()
{
	// todo: this canvas maybe no scene owner (current canvas count == 1)
	auto* canvasPtr = GetCurrentAnimCanvas();

	mRegistry.view<BBoxControlComponent>().each([](auto entity, BBoxControlComponent& bbox) { bbox.bbox->onUpdate(); });

	if (canvasPtr)
	{
		auto* animCanvas = static_cast<AnimationCreatorCanvas*>(canvasPtr);
		auto* animator = animCanvas->mAnimator.get();

		// todo: keyframe update logic here, mIsStop == true
		if (!animator->mIsStop || animator->mDirty)
		{
			const auto keyframeNo = animator->mCurrentFrameNo;
			// todo: other keyframe
			mRegistry.view<TransformComponent, TransformKeyframeComponent>().each(
				[keyframeNo](auto entity, TransformComponent& transform, TransformKeyframeComponent& keyframes)
				{
					transform.localCenterPosition = keyframes.positionKeyframes.frame(keyframeNo);
					transform.scale = keyframes.scaleKeyframes.frame(keyframeNo);
					transform.rotation = keyframes.rotationKeyframes.frame(keyframeNo);
				});
			animator->mDirty = false;
		}
	}

	mRegistry.view<TransformComponent, PathComponent, ShapeComponent>().each(
		[](auto entity, TransformComponent& transform, PathComponent& path, ShapeComponent& shape)
		{
			transform.update();
			shape.shape->reset();
			shape.shape->transform(transform.transform);
			auto pathPoint = path.points;
			// todo: path update
			shape.shape->appendPath(&path.pathCommands[0], path.pathCommands.size(), &pathPoint[0], path.points.size());
		});
	mRegistry.view<TransformComponent, ElipsePathComponent, ShapeComponent>().each(
		[](auto entity, TransformComponent& transform, ElipsePathComponent& path, ShapeComponent& shape)
		{
			transform.update();
			shape.shape->reset();
			shape.shape->transform(transform.transform);
			shape.shape->appendCircle(path.position.x, path.position.y, path.scale.x * 0.5f, path.scale.y * 0.5f);
		});
	mRegistry.view<TransformComponent, RectPathComponent, ShapeComponent>().each(
		[](auto entity, TransformComponent& transform, RectPathComponent& path, ShapeComponent& shape)
		{
			transform.update();
			shape.shape->reset();
			shape.shape->transform(transform.transform);
			shape.shape->appendRect(path.position.x - path.scale.x / 2.0f, path.position.y - path.scale.y / 2.0f,
									path.scale.x, path.scale.y, path.radius, path.radius);
		});
	mRegistry.view<ShapeComponent, SolidFillComponent>().each(
		[](auto entity, ShapeComponent& shape, SolidFillComponent& fill)
		{
			shape.shape->fill(fill.color.x, fill.color.y, fill.color.z, fill.alpha);
			shape.shape->fillRule(fill.rule);
		});
	mRegistry.view<ShapeComponent, StrokeComponent>().each(
		[](auto entity, ShapeComponent& shape, StrokeComponent& stroke)
		{
			shape.shape->strokeWidth(stroke.width);
			shape.shape->strokeFill(stroke.color.x, stroke.color.y, stroke.color.z, stroke.alpha);
		});
	mRegistry.view<SceneComponent>().each(
		[this](auto entity, SceneComponent& scene)
		{
			if (scene.scene->mId != this->mId)
			{
				scene.scene->onUpdate();
			}
		});
}

}	 // namespace core