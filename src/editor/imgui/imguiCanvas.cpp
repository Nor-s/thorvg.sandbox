#include "imguiCanvas.h"
#include <core/core.h>

#include "examples.h"

namespace editor
{
core::CanvasWrapper* ImGuiCanvasView::gCurrentCanvas = nullptr;

void ImGuiCanvasView::onDraw(std::string_view title, core::CanvasWrapper& canvas)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
	if (ImGui::Begin(title.data(), 0, ImGuiWindowFlags_NoScrollbar))
	{
		ImVec2 canvasSize = ImGui::GetContentRegionAvail();
		ImGui::ImageWithBg(canvas.getTexture(), ImVec2(canvas.mSize.x, canvas.mSize.y), ImVec2{0, 1}, ImVec2{1, 0});
		canvas.mSize.x = max(1.0f, canvasSize.x);
		canvas.mSize.y = max(1.0f, canvasSize.y);
		if (ImGui::IsWindowFocused())
		{
			gCurrentCanvas = &canvas;
		}
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void ImGuiCanvasView::onDrawSceneInspect()
{
	if (ImGui::Begin("scene properties", 0, 0))
	{
		if (gCurrentCanvas != nullptr)
		{
			if (gCurrentCanvas && gCurrentCanvas->isExampleCanvas())
			{
				auto* exampleCanvas = static_cast<tvgexam::ExampleCanvas*>(gCurrentCanvas);

				static int currentExampleIndex = 0;
				int beforeExampleIdx = currentExampleIndex;
				ImGui::Combo(
					"examples", &currentExampleIndex,
					[](void* data, int n) { return tvgexam::ExampleCanvas::gExampleList[n]->toString().data(); },
					nullptr, tvgexam::ExampleCanvas::gExampleList.size());

				if (beforeExampleIdx != currentExampleIndex)
				{
					exampleCanvas->changeExample(currentExampleIndex);
				}
			}

			ImGui::Separator();

			if (ImGui::Button("Square"))
			{
			}
			if (ImGui::Button("Circle"))
			{
			}
			if (ImGui::Button("SVG"))
			{
			}
			if (ImGui::Button("Lottie"))
			{
			}
		}
	}
	ImGui::End();
}

void ImGuiCanvasView::onDrawContentBrowser()
{
}

}	 // namespace editor