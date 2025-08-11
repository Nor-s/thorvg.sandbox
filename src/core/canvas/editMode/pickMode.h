#ifndef _CORE_CANVAS_EDITMODE_PICKMODE_H_
#define _CORE_CANVAS_EDITMODE_PICKMODE_H_

#include "editMode.h"
#include "scene/entity.h"

namespace core
{
class Scene;
class PickMode : public EditMode
{
	struct Context
	{
		// for outline
		std::unique_ptr<Scene> tempScene;
		std::vector<Entity> pickedEntity;
		Vec2 startPoint;
		Entity bbox;
		Entity drag;
	};

public:
    PickMode(AnimationCreatorCanvas* canvas);
    ~PickMode();
	void onStarClickLefttMouse(const InputValue& inputValue) override;
	void onDragLeftMouse(const InputValue& inputValue) override;
	void onEndLeftMouse(const InputValue& inputValue) override;
	void onInputDetach(const InputValue& inputValue) override;

private:
	AnimationCreatorCanvas* rCanvas;
	Context mContext;
};
}

#endif