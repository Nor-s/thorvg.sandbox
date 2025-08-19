#ifndef _CORE_CANVAS_EDITMODE_PICKMODE_H_
#define _CORE_CANVAS_EDITMODE_PICKMODE_H_

#include "editMode.h"
#include "scene/entity.h"
#include "../shapeUtil.h"

namespace core
{
class Scene;
class Bbox;

class PickMode : public EditMode
{
	struct Context
	{
		// for outline
		std::unique_ptr<Scene> tempScene;
		PickInfo pickInfo;
		Vec2 startPoint;
		Vec2 beforePoint;

		std::unique_ptr<Bbox> bbox = nullptr;
		Entity hover;
		// Entity bbox;
		// Entity currentObb;
		// Entity drag;
		bool isLeftMouseDown{false};
	};

public:
    PickMode(AnimationCreatorCanvas* canvas);
    ~PickMode();
	void onUpdate() override;
	bool pick(const InputValue& inputValue, tvg::Paint* paint, int depth);
	bool pick(const InputValue& inputValue);
	bool onStarClickLefttMouse(const InputValue& inputValue) override;
	bool onDragLeftMouse(const InputValue& inputValue) override;
	bool onMoveMouse(const InputValue& inputValue) override;
	bool onEndLeftMouse(const InputValue& inputValue) override;
	bool onInputDetach(const InputValue& inputValue) override;

private:
	AnimationCreatorCanvas* rCanvas;
	Context mContext;
};
}

#endif