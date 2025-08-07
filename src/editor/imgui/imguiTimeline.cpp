#include "imguiTimeline.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_neo_sequencer.h>
#include <icons/icons.h>

#include <core/core.h>

namespace editor
{

void ImguiTimeline::draw(core::AnimationBuilder* builder)
{
	ImGuiWindowFlags windowFlags = 0;
    rBuilder = builder;

	if (mIsHoveredZoomSlider)
	{
		windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
	}
	ImGui::Begin(ICON_MD_SCHEDULE " Animation", 0, windowFlags);
	{
		ImGui::BeginChild("##Timeline", ImVec2(0, 0), false, windowFlags);
		{
			drawSequencer();
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void ImguiTimeline::drawSequencer()
{
	// uint32_t current = static_cast<uint32_t>(rBuilder->mCurrentFrame);
	// uint32_t start = static_cast<uint32_t>(0);
	// uint32_t end = static_cast<uint32_t>(rBuilder->totalFrame());
	// uint32_t before = current;

	// auto win_pos = ImGui::GetWindowPos();
	// auto viewportPanelSize = ImGui::GetContentRegionAvail();
	// win_pos.y = ImGui::GetCursorPosY();
	// if (ImGui::BeginNeoSequencer("Sequencer", &current, &start, &end))
	// {
	// 	// draw_keyframes(ui_context, anim_component->get_animation());

	// 	mIsHoveredZoomSlider = ImGui::IsZoomSliderHovered();
	//     ImGui::EndNeoSequencer();
	// }

	// // update current time
	// if (before != current)
	// {
    //     rBuilder->mCurrentFrame = current;
	// }
    
}

}	 // namespace editor