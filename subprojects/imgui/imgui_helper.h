#ifndef IMGUI_HELPER_H
#define IMGUI_HELPER_H

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include <string>

namespace ImGui::Helper
{

class ButtonColorSetter
{
public:
	ButtonColorSetter(ImVec4 btn_color);
	~ButtonColorSetter();
};

void HelpMarker(const char* desc);
inline void BeginDragProperty(const char* label, const ImVec2& btn_size = {0.0f, 0.0f});
inline void BeginDragPropertyWithoutColor(const char* label, const ImVec2& btn_size = {0.0f, 0.0f});
inline void EndDragProperty();
inline void EndDragPropertyWithoutColor();

bool DragFPropertyXYZ(const char* label,
					  float* value,
					  float step = 0.01f,
					  float min = -1000.0f,
					  float max = 1000.0f,
					  const char* format = "%.2f",
					  const std::string& help_message = "",
					  int num = 3);
bool DragPropertyXYZ(const char* label,
					 float* value,
					 float step = 0.01f,
					 float min = -1000.0f,
					 float max = 1000.0f,
					 const std::string& help_message = "");
bool DragFloatProperty(const char* label,
					   float& value,
					   float step = 1.0f,
					   float min = -1000.0f,
					   float max = 1000.0f,
					   const ImVec2& btn_size = {0.0f, 0.0f},
					   const char* format = "%.0f",
					   const std::string& help_message = "");

bool DragFloatPropertyWithColor(const char* label,
								float& value,
								const ImVec4& btn_color,
								const ImVec4& frame_color,
								float step = 1.0f,
								float min = -1000.0f,
								float max = 1000.0f,
								const ImVec2& btn_size = {0.0f, 0.0f},
								const char* format = "%.0f",
								const std::string& help_message = "");

bool DragIntProperty(const char* label,
					 int& value,
					 float step = 1,
					 int min = 0,
					 int max = 1000,
					 const ImVec2& btn_size = {0.0f, 0.0f},
					 const std::string& help_message = "");

bool ToggleButton(const char* label, bool* v, const ImVec2& size = {0.0f, 0.0f});
bool ToggleButtonImage(const char* label, bool bIsToggle, const ImVec2& size = {0.0f, 0.0f});
}	 // namespace ImGui::Helper

#endif