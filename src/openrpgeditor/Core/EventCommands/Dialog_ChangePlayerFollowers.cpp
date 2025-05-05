#include "Core/EventCommands/Dialog_ChangePlayerFollowers.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <tuple>

std::tuple<bool, bool> Dialog_ChangePlayerFollowers::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 5) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::BeginVertical("##change_player_followers_vert_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox operationGroupBox(trNOOP("Player Followers"), "##change_player_followers_layout", {-1, 0}, nullptr,
                                 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_player_followers_group_horizontal", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(.5);
          ImGui::RadioButton(trNOOP("ON"), &m_checkIfOn, 0);
          ImGui::RadioButton(trNOOP("OFF"), &m_checkIfOn, 1);
          ImGui::Spring(.5);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_player_followers_dialog_buttons", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_player_followers_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          command->followersEnabled = m_checkIfOn == 0 ? ValueControl::ON : ValueControl::OFF;
          ImGui::CloseCurrentPopup();
          setOpen(false);
          m_confirmed = true;
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
