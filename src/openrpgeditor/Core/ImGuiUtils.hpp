#pragma once
#include "imgui.h"
namespace ImGui {
void BeginGroupPanel(const char* name = "", const ImVec2& size = ImVec2(-1.0f, -1.0f));
void EndGroupPanel();
bool LabelOverLineEdit(const char* id, const char* label, char* string, int len, float width,
                       const char* tooltip = nullptr);

/* Copy of ImGui::Selectable with borders enabled */
bool SelectableWithBorder(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

void ActionTooltip(const char* action, const char* fmt, ...);
} // namespace ImGui