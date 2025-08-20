#ifndef _CORE_CANVAS_EDITMODE_ADDMODE_H_
#define _CORE_CANVAS_EDITMODE_ADDMODE_H_

#include "editMode.h"
#include "scene/entity.h"

namespace core
{
class Scene;

class AddMode : public EditMode
{
	struct Context
	{
		// for outline
		std::unique_ptr<core::Scene> tempScene{nullptr};
		Entity newEntity{};
		Vec2 startPoint{};
	};

public:
	AddMode(AnimationCreatorCanvas* canvas, EditModeType type);
	~AddMode();
	void onUpdate() override;
	bool onStarClickLefttMouse(const InputValue& inputValue) override;
	bool onDragLeftMouse(const InputValue& inputValue) override;
	bool onEndLeftMouse(const InputValue& inputValue) override;
	bool onInputDetach(const InputValue& inputValue) override;

private:
	AnimationCreatorCanvas* rCanvas{nullptr};
	Context mContext{};
	EditModeType mType{};
};
}	 // namespace core

#endif