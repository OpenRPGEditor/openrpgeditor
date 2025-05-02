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
bool SizeableCheckbox(const char* label, bool* v, float height = 0);
bool BeginGroupBox(const char* label, const ImVec2& size = {0, 0}, bool* selected = nullptr, bool* clicked = nullptr, ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0);
void EndGroupBox();

void ItemLabel(const char* title, ImGuiItemLabelFlags flags = ImGuiItemLabelFlags_Default);
bool InputIntFormat(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0, const char* fmt = nullptr);
bool SpinScaler(const char* label, ImGuiDataType data_type, void* data_ptr, const void* step, const void* step_fast, const char* format, ImGuiInputTextFlags flags);
bool SpinInt(const char* label, int* v, int step = 1, int step_fast = 100, const char* fmt = nullptr, ImGuiInputTextFlags flags = 0);
bool SpinFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
bool SpinDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0);
} // namespace ImGui