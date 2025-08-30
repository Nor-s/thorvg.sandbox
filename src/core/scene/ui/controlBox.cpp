#include "controlBox.h"

#include "common/common.h"
#include "scene/component/components.h"
#include "scene/component/uiComponents.h"

#include "canvas/shapeUtil.h"

namespace core
{

ControlBox::ControlBox(Scene* scene, Vec2 center, Vec2 wh, Type type, ShapeType shapeType)
{
	rScene = scene;
    switch(shapeType)
    {
        case ShapeType::FillStrokeEllipse:
        case ShapeType::TransparentEllipse:
        case ShapeType::StrokeEllipse:
        {
            mEntity = rScene->createEllipseFillStrokeLayer(center - wh/2.0f, wh);
            break;
        }
        case ShapeType::FillStrokeRect:
        case ShapeType::StrokeRect:
        {
            mEntity = rScene->createRectFillStrokeLayer(center - wh/2.0f, wh);
            break;
        }
    }
    auto& stroke = mEntity.getComponent<StrokeComponent>();
    auto& fill = mEntity.getComponent<SolidFillComponent>();
    stroke.color = CommonSetting::Color_DefaultControlBoxOutline;
    fill.color = CommonSetting::Color_DefaultControlBoxInner;

    switch(shapeType)
    {
        case ShapeType::StrokeEllipse:
        case ShapeType::StrokeRect:
        case ShapeType::TransparentEllipse:
        {
            fill.alpha = 0.0f;
        }
    }
    switch(shapeType)
    {
        case ShapeType::TransparentEllipse:
        {
            stroke.alpha = 0.0f;
        }
    }

    auto& shape = mEntity.getComponent<ShapeComponent>();
    mObbPoints = GetObb(shape.shape);
}

ControlBox::ControlBox(Scene* scene, const std::array<Vec2, 4>& obbPoints, Type type)
{
	rScene = scene;
    mObbPoints = obbPoints;
    mEntity = rScene->createObb(obbPoints);

    auto& stroke = mEntity.getComponent<StrokeComponent>();
    stroke.color = CommonSetting::Color_DefaultControlBoxOutline;
}

ControlBox::~ControlBox()
{
	rScene->destroyEntity(mEntity);
}

void ControlBox::moveTo(const Vec2& xy)
{
    auto& transform = mEntity.getComponent<TransformComponent>();
    transform.localCenterPosition = xy;
    mEntity.update();

    auto& shape = mEntity.getComponent<ShapeComponent>();
    mObbPoints = GetObb(shape.shape);
}
void ControlBox::moveByDelta(const Vec2& delta)
{
    mEntity.moveByDelta(delta);
    mEntity.update();

    auto& shape = mEntity.getComponent<ShapeComponent>();
    mObbPoints = GetObb(shape.shape);
}
bool ControlBox::onLeftDown(Vec2 xy)
{
    if(IsInner(mObbPoints, xy))
    {
        return true;
    }
    return false;
}

void ControlBox::setVisible(bool visible)
{
	visible ? mEntity.show() : mEntity.hide();
}

bool ControlBox::isVisible() const
{
	return !mEntity.isHidden();
}

}	 // namespace core