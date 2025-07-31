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
	static core::CanvasWrapper* gCurrentCanvas;
	void onDraw(std::string_view title, core::CanvasWrapper& canvas);
	void onDrawSceneInspect();
	void onDrawContentBrowser();
};

}	 // namespace editor

#endif