#ifndef _EDITOR_IMGUI_IMGUI_SHAPE_PANEL_H_
#define _EDITOR_IMGUI_IMGUI_SHAPE_PANEL_H_

namespace core
{
class AnimationCreatorCanvas;
}

namespace editor
{
class ImGuiShapePanel
{
public:
	void draw(core::AnimationCreatorCanvas* canvas);
};

}	 // namespace editor

#endif