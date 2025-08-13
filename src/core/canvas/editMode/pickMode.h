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
		tvg::Paint* currentSelectedPaint{nullptr};
		Scene* currentSelectedScene{nullptr};
		Vec2 startPoint;
		Vec2 beforePoint;
		Entity bbox;
		Entity currentObb;
		Entity drag;
		Entity hover;
		bool isLeftMouseDown{false};
	};

public:
    PickMode(AnimationCreatorCanvas* canvas);
    ~PickMode();
	bool pick(const InputValue& inputValue, tvg::Paint* paint, int depth);
	void onStarClickLefttMouse(const InputValue& inputValue) override;
	void onDragLeftMouse(const InputValue& inputValue) override;
	void onMoveMouse(const InputValue& inputValue) override;
	void onEndLeftMouse(const InputValue& inputValue) override;
	void onInputDetach(const InputValue& inputValue) override;

private:
	AnimationCreatorCanvas* rCanvas;
	Context mContext;
};
}

#endif