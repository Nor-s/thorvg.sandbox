#define IMGUI_DEFINE_MATH_OPERATORS
#include "imguiCanvas.h"
#include <core/core.h>

#include <ImGuiNotify.hpp>
#include <imgInspect.h>

#include "examples.h"
#include <core/canvas/canvas.h>

#include <filesystem>



namespace editor
{
core::CanvasWrapper* ImGuiCanvasView::gCurrentCanvas = nullptr;

void ImGuiCanvasView::onDraw(std::string_view title, core::CanvasWrapper& canvas)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
	if (ImGui::Begin(title.data(), 0, ImGuiWindowFlags_NoScrollbar))
	{
		ImVec2 canvasSize = ImGui::GetContentRegionAvail();
		auto textureSize = ImVec2(canvas.mSize.x, canvas.mSize.y);
		ImGui::ImageWithBg(canvas.getTexture(), textureSize, ImVec2{0, 1}, ImVec2{1, 0});

		if (ImGui::IsWindowFocused())
		{
			gCurrentCanvas = &canvas;
		}

		auto& io = ImGui::GetIO();
		ImRect rc = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
		ImVec2 mouseUVCoord = (io.MousePos - rc.Min) / rc.GetSize();
		mouseUVCoord.y = 1.f - mouseUVCoord.y;

		if (io.KeyShift && mouseUVCoord.x >= 0.f && mouseUVCoord.y >= 0.f)
		{
			ImageInspect::inspect(canvas.mSize.x, canvas.mSize.y, canvas.getBuffer(), mouseUVCoord, textureSize);
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				auto* path = (const char*) payload->Data;
				auto ext = std::filesystem::path(path).extension();
				ImGui::InsertNotification({ImGuiToastType::Info, 3000, "File: %s", path});
				if(ext.string() == ".json")
				{
					auto [animWrap, pictureWrap] = core::AnimationWrapper::Gen(path);
					canvas.pushPaint(std::move(pictureWrap));
					canvas.pushAnimation(std::move(animWrap));
				}
				if(ext.string() == ".png" ||ext.string() == ".svg" || ext.string() == ".jpg" || ext.string() == ".webp")
				{
					canvas.pushPaint(core::PictureWrapper::Gen(path));
				}
			}
			ImGui::EndDragDropTarget();
		}

		// fore canvas resize
		canvas.mSize.x = max(1.0f, canvasSize.x);
		canvas.mSize.y = max(1.0f, canvasSize.y);
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

				int currentExampleIndex = exampleCanvas->mCurrentExampleIdx;
				int beforeExampleIdx = currentExampleIndex;
				ImGui::Combo(
					"examples", &currentExampleIndex,
					[](void* data, int n) { return tvgexam::ExampleCanvas::gExampleList[n]->toString().data(); },
					nullptr, tvgexam::ExampleCanvas::gExampleList.size());

				if (beforeExampleIdx != currentExampleIndex)
				{
					exampleCanvas->mCurrentExampleIdx = currentExampleIndex;
				}
			}

			ImGui::Separator();
		}
	}
	ImGui::End();
}

void ImGuiCanvasView::onDrawContentBrowser()
{
}

}	 // namespace editor