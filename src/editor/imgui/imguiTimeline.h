#ifndef _EDITOR_IMGUI_IMGUI_TIMELINE_H_
#define _EDITOR_IMGUI_IMGUI_TIMELINE_H_

#include <vector>
#include <memory>
#include <string>

namespace core
{
    class AnimationBuilder;
}

namespace editor
{
class ImguiTimeline
{
public:
    void draw(core::AnimationBuilder* builder);

private:
    void drawSequencer();

private:
    core::AnimationBuilder* rBuilder = nullptr;
    bool mIsHoveredZoomSlider = false;

};

}

#endif