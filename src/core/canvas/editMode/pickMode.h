#ifndef _CORE_CANVAS_EDITMODE_PICKMODE_H_
#define _CORE_CANVAS_EDITMODE_PICKMODE_H_

#include "editMode.h"

namespace core
{
class PickMode : public EditMode
{
	struct Context
	{
		std::vector<tvg::Paint*> rPickedPaint;
		// for outline
		tvg::Scene* tempScene{nullptr};
		tvg::Shape* mOutline{nullptr};
		tvg::Shape* mScalePoint[4]{nullptr, nullptr, nullptr, nullptr};
        tvg::Paint* mPick{nullptr};
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