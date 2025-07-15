#include "imguiCanvas.h"

#include <core/core.h>

namespace editor
{

void ImGuiCanvasView::onDraw(std::string_view title, core::CanvasWrapper& canvas)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
	if (ImGui::Begin(title.data(), 0, ImGuiWindowFlags_NoScrollbar))
	{
		ImVec2 canvasSize = ImGui::GetContentRegionAvail();
		ImGui::ImageWithBg(canvas.getTexture(), ImVec2(canvas.mSize.x, canvas.mSize.y), ImVec2{0, 1}, ImVec2{1, 0});
		canvas.mSize.x = max(1.0f, canvasSize.x);
		canvas.mSize.y = max(1.0f, canvasSize.y);
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

}	 // namespace editor