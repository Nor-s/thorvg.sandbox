#define IMGUI_DEFINE_MATH_OPERATORS

#include "../app.h"
#include "event/events.h"

#include "imguiCanvas.h"
#include "imguiTimeline.h"
#include "imguiShapePanel.h"
#include "examples.h"

#include <core/core.h>

#include <ImGuiNotify.hpp>
#include <imgInspect.h>
#include <imgui_internal.h>
#include <filesystem>

namespace editor
{
core::CanvasWrapper* ImGuiCanvasView::gCurrentCanvas = nullptr;

void ImGuiCanvasView::onDraw(std::string_view title, core::CanvasWrapper& canvas, int canvasIndex)
{
	static ImGuiWindowFlags windowFlags = 0;

	auto& io = ImGui::GetIO();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});

	if (ImGui::Begin(title.data(), 0, windowFlags | ImGuiWindowFlags_NoScrollbar ))
	{
		bool isDraggingTitle = ImGui::IsItemHovered() && ImGui::IsItemClicked();
		bool isMoving = ImGui::GetCurrentContext()->MovingWindow == ImGui::GetCurrentWindow();
		ImVec2 canvasSize = ImGui::GetContentRegionAvail();
		auto textureSize = ImVec2(canvas.mSize.x, canvas.mSize.y);
		ImGui::ImageWithBg(canvas.getTexture(), textureSize, ImVec2{0, 1}, ImVec2{1, 0});

		if (gCurrentCanvas == nullptr || ImGui::IsWindowFocused())
		{
			gCurrentCanvas = &canvas;
		}

		auto rc = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
		bool isMouseHoveringRect = rc.Contains({io.MousePos, io.MousePos});
		auto mousePosition = io.MousePos - rc.Min;
		auto mouseUVCoord = mousePosition / rc.GetSize();
		mouseUVCoord.y = 1.f - mouseUVCoord.y;

		// set mouseOffset for fit canvas
		if(ImGui::IsWindowFocused() && isMouseHoveringRect)
		{
			core::io::mouseOffset = {-rc.Min.x, -rc.Min.y};
		}

		// draw texture inspector
		if (isMouseHoveringRect && io.KeyShift && mouseUVCoord.x >= 0.f && mouseUVCoord.y >= 0.f)
		{
			ImageInspect::inspect(canvas.mSize.x, canvas.mSize.y, canvas.getBuffer(), mouseUVCoord, textureSize);
		}

		// import
		// todo: import event
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				auto* path = (const char*) payload->Data;
				auto ext = std::filesystem::path(path).extension();
				ImGui::InsertNotification({ImGuiToastType::Info, 3000, "File: %s", path});
				if (ext.string() == ".json")
				{
					auto [animWrap, pictureWrap] = core::AnimationWrapper::Gen(path);
					canvas.pushPaint(std::move(pictureWrap));
					canvas.pushAnimation(std::move(animWrap));
				}
				if (ext.string() == ".png" || ext.string() == ".svg" || ext.string() == ".jpg" ||
					ext.string() == ".webp")
				{
					canvas.pushPaint(core::PictureWrapper::Gen(path));
				}
			}
			ImGui::EndDragDropTarget();
		}

		// todo: push when change focus, resize
		auto updateSize = core::Size{max(1.0f, canvasSize.x), max(1.0f, canvasSize.y)};
		bool needResize = canvasSize.x != textureSize.x || canvasSize.y != textureSize.y;
		App::PushEvent<CanvasFocusEvent>(canvasIndex, ImGui::IsWindowFocused()&&!isDraggingTitle&&!needResize&&!isMoving);
		if(needResize)
			App::PushEvent<CanvasResizeEvent>(canvasIndex, updateSize);
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

void ImGuiCanvasView::onDrawSceneInspect()
{
	if (gCurrentCanvas == nullptr || gCurrentCanvas->type() != core::CanvasType::LottieCreator)
		return;

	auto* lottieCanvas = static_cast<core::AnimationCreatorCanvas*>(gCurrentCanvas);
	ImguiTimeline().draw(lottieCanvas);
	ImGuiShapePanel().draw(lottieCanvas);

	if (ImGui::Begin("scene properties", 0, 0))
	{
		if (gCurrentCanvas != nullptr)
		{
			if (gCurrentCanvas && gCurrentCanvas->type() == core::CanvasType::Example)
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
				if (ImGui::Button("clear"))
				{
					exampleCanvas->onInit();
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