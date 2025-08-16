#include "controlBox.h"

#include "scene/component/components.h"

namespace core
{

ControlBox::ControlBox(Scene* scene, Vec2 xy, Vec2 wh, Type type)
{
	rScene = scene;
	mEntity = rScene->createRectFillStrokeLayer("ControlBox", xy - wh/2.0f, wh);
    auto& fill = mEntity.getComponent<SolidFillComponent>();
    fill.color = {245.0f, 245.0f, 245.0f};
}

ControlBox::~ControlBox()
{
	rScene->destroyEntity(mEntity);
}

void ControlBox::moveTo(Vec2 xy)
{
    auto& transform = mEntity.getComponent<TransformComponent>();
    transform.position = xy;
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