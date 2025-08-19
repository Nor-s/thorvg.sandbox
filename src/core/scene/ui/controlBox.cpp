#include "controlBox.h"

#include "common/common.h"
#include "scene/component/components.h"

#include "canvas/shapeUtil.h"

namespace core
{

ControlBox::ControlBox(Scene* scene, Vec2 center, Vec2 wh, Type type, ShapeType shapeType)
{
	rScene = scene;
    if (shapeType == ShapeType::Ellipse)
    {
        mEntity = rScene->createEllipseFillStrokeLayer(center - wh/2.0f, wh);
    }
    else
    {
        mEntity = rScene->createRectFillStrokeLayer(center - wh/2.0f, wh);
    }
    auto& stroke = mEntity.getComponent<StrokeComponent>();
    auto& fill = mEntity.getComponent<SolidFillComponent>();
    stroke.color = Style::ControlBoxOutlineColor;

    switch(type)
    {
        case Type::Move:
        {
            fill.alpha = 0.0f;
            break;
        }
        case Type::Rotate:
        {
            fill.alpha = 0.0f;
            stroke.alpha = 0.0f;
            break;
        }
        case Type::Scale:
        {
            auto& fill = mEntity.getComponent<SolidFillComponent>();
            fill.color = Style::ControlBoxInnerColor;
            break;
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
    stroke.color = Style::ControlBoxOutlineColor;
}

ControlBox::~ControlBox()
{
	rScene->destroyEntity(mEntity);
}

void ControlBox::moveTo(Vec2 xy)
{
    auto& transform = mEntity.getComponent<TransformComponent>();
    transform.localCenterPosition = xy;
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