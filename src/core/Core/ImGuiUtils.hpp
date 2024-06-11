#pragma once
#include "imgui.h"
namespace ImGui {
void BeginGroupPanel(const char* name = "", const ImVec2& size = ImVec2(-1.0f, -1.0f));
void EndGroupPanel();
bool LabelOverLineEdit(const char* id, const char* label, char* string, int len, float width,
                       const char* tooltip = nullptr);
} // namespace ImGui