#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <format>
#include <imgui_internal.h>

GroupBox::GroupBox(const std::string_view title, const std::string_view id, const ImVec2& size, bool* checked, ImGuiChildFlags childFlags, ImGuiWindowFlags windowFlags)
: m_title(title)
, m_id(id)
, m_size(size)
, m_childFlags(childFlags)
, m_windowFlags(windowFlags)
, m_checked(checked)
, m_visible(false) {}

bool GroupBox::begin() {
  m_beginCalled = true;
  ImGui::BeginChild(std::format("{}_outer", m_id).c_str(), m_size, ImGuiChildFlags_NavFlattened | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                    ImGuiWindowFlags_NoBackground);
  const auto id = ImGui::GetID(m_id.c_str());
  auto groupPos = ImGui::GetCursorScreenPos();
  m_groupStart = groupPos;
  if (!m_title.empty()) {
    ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.y * 0.5);
    groupPos.y += (ImGui::GetFontSize() / 2) + ImGui::GetStyle().ItemSpacing.y;
    ImGui::SetCursorScreenPos(groupPos);
    groupPos.y += (ImGui::GetFrameHeight() / 2);
    ImGui::PopStyleVar();
  }
  m_clicked = false;

  m_visible = ImGui::BeginChild(id, m_size,
                                m_childFlags | ImGuiChildFlags_FrameStyle | ImGuiChildFlags_NavFlattened | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                                m_windowFlags);
  if (m_visible) {
    if (!m_wasHeaderDrawn && !m_title.empty()) {
      auto size = ImGui::CalcItemSize(ImGui::CalcTextSize(m_title.c_str()), 0, 0);
      if (m_checked) {
        ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.y * 0.5);
        size.x += (ImGui::GetFrameHeightWithSpacing() * 0.45f) * 2;
        ImGui::PopStyleVar();
      }
      ImGui::Dummy({size.x, size.y / 2});
    }
    ImGui::BeginDisabled(m_checked && !*m_checked);
  }

  return m_visible;
}

void GroupBox::end() {
  assert(m_beginCalled && "GroupBox::begin not called");

  ImRect clip;

  if (m_visible) {
    ImGui::EndDisabled();
    clip = ImGui::GetCurrentWindow()->ClipRect;
    ImGui::EndChild();
  }

  if (m_visible && !m_wasHeaderDrawn && !m_title.empty()) {
    const auto labelId = ImGui::GetID(std::format("{}_checkbox", m_id).c_str());
    const auto oldPos = ImGui::GetCursorScreenPos();
    const auto oldX = ImGui::GetCursorPosX();
    ImGui::SetCursorScreenPos({m_groupStart.x + ImGui::GetStyle().FramePadding.x + ImGui::GetStyle().FrameBorderSize, m_groupStart.y});
    clip.Min.y -= ImGui::GetFrameHeightWithSpacing() / 2;
    ImGui::PushClipRect(clip.Min, clip.Max, false);
    if (ImGui::BeginChild(labelId, {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle | ImGuiChildFlags_NavFlattened,
                          ImGuiWindowFlags_NoScrollbar | (!m_checked ? ImGuiWindowFlags_NoNav : 0))) {
      // Group Header text
      if (!m_checked) {
        ImGui::TextUnformatted(m_title.c_str());
      } else {
        ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.y * 0.5);
        m_clicked = ImGui::SizeableCheckbox(m_title.c_str(), m_checked, ImGui::GetFrameHeight() * 0.45f);
        ImGui::PopStyleVar();
      }
      ImGui::EndChild();
    }
    ImGui::PopClipRect();
    ImGui::SetCursorScreenPos(oldPos);
    ImGui::SetCursorPosX(oldX);
    m_wasHeaderDrawn = true;
  }
  ImGui::EndChild();
  m_beginCalled = false;
}
