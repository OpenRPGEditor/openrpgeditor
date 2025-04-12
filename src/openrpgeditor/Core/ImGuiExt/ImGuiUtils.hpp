#pragma once
#include "imgui.h"
#include <string>
#include <vector>

enum ImGuiItemLabelFlags {
  ImGuiItemLabelFlags_Left = 1u << 0u,
  ImGuiItemLabelFlags_Right = 1u << 1u,
  ImGuiItemLabelFlags_Default = ImGuiItemLabelFlags_Left,
};

namespace ImGui {
void BeginGroupPanel(const char* name = "", const ImVec2& size = ImVec2(-1.0f, -1.0f));
void EndGroupPanel();
bool LabelOverLineEdit(const char* id, const char* label, char* string, int len, float width, const char* tooltip = nullptr, ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue);

/* Copy of ImGui::Selectable with borders enabled */
bool SelectableWithBorder(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

void ActionTooltip(const char* action, const char* fmt, ...);

int ButtonGroup(const char* id, const std::vector<std::string>& buttons, bool isVertical = false);

ImVec2 GetDPIScaledSize(const ImVec2& size);
inline ImVec2 GetDPIScaledSize(const float w, const float h) { return GetDPIScaledSize(ImVec2(w, h)); }

float GetDPIScaledValue(float value);
bool MenuItemNoCheck(const char* label, const char* icon, const char* shortcut = NULL, bool selected = false, bool enabled = true);

float GetMinimumPanelHeight();

bool BeginGroupBox(const char* str_id, const char* title, const ImVec2& size = {0, 0});
void EndGroupBox();

void ItemLabel(const char* title, ImGuiItemLabelFlags flags = ImGuiItemLabelFlags_Default);
} // namespace ImGui