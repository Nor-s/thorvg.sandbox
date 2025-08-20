#ifndef _EDITOR_IMGUI_IMGUI_TIMELINE_H_
#define _EDITOR_IMGUI_IMGUI_TIMELINE_H_

#include <vector>
#include <memory>
#include <string>

#include <imgui.h>

namespace core
{
    class AnimationCreatorCanvas;
}

namespace editor
{
class ImguiTimeline
{
public:
    void draw(core::AnimationCreatorCanvas* canvas);

private:
    void drawSequencer();

private:
    core::AnimationCreatorCanvas* rCanvas = nullptr;
    bool mIsHoveredZoomSlider = false;
   	ImGuiWindowFlags mWindowFlags = 0;

};

}

#endif