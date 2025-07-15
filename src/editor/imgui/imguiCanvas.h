#ifndef _EDITOR_IMGUI_IMGUICANVAS_H_
#define _EDITOR_IMGUI_IMGUICANVAS_H_

#include <string_view>

namespace core
{
class CanvasWrapper;
}

namespace editor
{

class ImGuiCanvasView
{
public:
	void onDraw(std::string_view title, core::CanvasWrapper& canvas);
};

}	 // namespace editor

#endif