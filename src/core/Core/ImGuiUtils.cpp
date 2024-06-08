#include "Core/ImGuiUtils.hpp"
#include "imgui_internal.h"

namespace ImGui {
void BeginGroupPanel(const char* name, const ImVec2& size) {
  BeginGroup();

  auto itemSpacing = GetStyle().ItemSpacing;
  PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
  PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

  auto frameHeight = GetFrameHeight();
  BeginGroup();

  ImVec2 effectiveSize = size;
  if (size.x < 0.0f)
    effectiveSize.x = GetContentRegionAvail().x;
  else
    effectiveSize.x = size.x;
  Dummy(ImVec2(effectiveSize.x, 0.0f));

  Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
  SameLine(0.0f, 0.0f);
  BeginGroup();
  Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
  SameLine(0.0f, 0.0f);
  TextUnformatted(name);
  SameLine(0.0f, 0.0f);
  Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
  BeginGroup();

  PopStyleVar(2);

  GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
  GetCurrentWindow()->Size.x -= frameHeight;

  PushItemWidth(effectiveSize.x - frameHeight);
}

void EndGroupPanel() {
  PopItemWidth();

  auto itemSpacing = GetStyle().ItemSpacing;

  PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
  PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

  auto frameHeight = GetFrameHeight();

  // workaround for incorrect capture of columns/table width by placing
  // zero-sized dummy element in the same group, this ensure
  // max X cursor position is updated correctly
  SameLine(0.0f, 0.0f);
  Dummy(ImVec2(0.0f, 0.0f));

  EndGroup();

  // GetWindowDrawList()->AddRectFilled(GetItemRectMin(), GetItemRectMax(), IM_COL32(0, 255, 0, 64), 4.0f);

  EndGroup();

  SameLine(0.0f, 0.0f);
  Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
  Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

  EndGroup();

  auto itemMin = GetItemRectMin();
  auto itemMax = GetItemRectMax();
  // GetWindowDrawList()->AddRectFilled(itemMin, itemMax, IM_COL32(255, 0, 0, 64), 4.0f);

  ImVec2 halfFrame = ImVec2(frameHeight * 0.25f, frameHeight) * 0.5f;
  GetWindowDrawList()->AddRect(itemMin + halfFrame, itemMax - ImVec2(halfFrame.x, 0.0f),
                               ImColor(GetStyleColorVec4(ImGuiCol_Border)), halfFrame.x);

  PopStyleVar(2);

  GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
  GetCurrentWindow()->Size.x += frameHeight;

  Dummy(ImVec2(0.0f, 0.0f));

  EndGroup();
}

bool LabelOverLineEdit(const char* id, const char* label, char* string, int len, float width) {
  BeginGroup();
  {
    Text("%s", label);
    char name[len];
    strncpy(name, string, len);
    ImGui::SetNextItemWidth(width);
    if (InputText(id, name, len, ImGuiInputTextFlags_EnterReturnsTrue)) {
      strncpy(string, name, len);
      return true;
    }
  }
  EndGroup();
  return false;
}
} // namespace ImGui