#include "Core/EventCommands/Dialog_JumpToLabel.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_JumpToLabel::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###JumpToLabel");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 5) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###JumpToLabel", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##jump_to_label_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox labelGroupBox(trNOOP("Label Name"), "##jump_to_label_label_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize);
      if (labelGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##label_input", &m_label);
      }
      labelGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##jump_to_label_buttons_layout", {-1, -1}, 0);
      {
        ImGui::Spring();
        ImGui::SetNextItemWidth(-1);
        if (const auto ret = ImGui::ButtonGroup("##jump_to_label_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->label = m_label;
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
