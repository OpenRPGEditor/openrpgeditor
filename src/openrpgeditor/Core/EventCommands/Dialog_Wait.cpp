#include "Core/EventCommands/Dialog_Wait.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_Wait::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###Wait");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 5) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###Wait", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##wait_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox labelGroupBox(trNOOP("Duration"), "##wait_duration_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize);
      if (labelGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::SpinInt("##wait_duration_input", &m_waitDuration, 1, 100, trNOOP("%4d frames (1/60 sec)"))) {
          m_waitDuration = std::clamp(m_waitDuration, 1, 253);
        }
      }
      labelGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##wait_buttons_layout", {-1, -1}, 0);
      {
        ImGui::Spring();
        ImGui::SetNextItemWidth(-1);
        if (const auto ret = ImGui::ButtonGroup("##wait_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->duration = m_waitDuration;
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}