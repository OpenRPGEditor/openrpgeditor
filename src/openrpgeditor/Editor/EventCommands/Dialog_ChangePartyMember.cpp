#include "Editor/EventCommands/Dialog_ChangePartyMember.hpp"

#include "Database/Database.hpp"

#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangePartyMember::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangePartyMember");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ChangePartyMember", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_party_member_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##change_party_member_actor_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (actorGroupBox.begin()) {
        ImGui::SetNextItemWidth(-1);
        ImGui::PushID("##change_party_member_actor_selection");
        if (ImGui::EllipsesButton(Database::instance()->actorNameAndId(m_actor).c_str(), ImVec2{-1, 0})) {
          m_actorPicker = ActorPicker( Database::instance()->actors->actorList(), m_actor);
          m_actorPicker->setOpen(true);
        }
        ImGui::PopID();
      }
      actorGroupBox.end();

      GroupBox operationGroupBox(trNOOP("Operation"), "##change_party_member_operation_group", {-1, 0}, nullptr,
                                 ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_party_member_operation_group_layout", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(.33f);
          ImGui::RadioButton("Add", &m_operation, 0);
          ImGui::Spring(.33f);
          ImGui::RadioButton("Remove", &m_operation, 1);
          ImGui::Spring(.33f);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_party_member_buttons", {-1, -1});
      {
        ImGui::BeginDisabled(m_operation != 0);
        ImGui::Checkbox("Initialize", &m_isInit);
        ImGui::EndDisabled();

        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_party_member_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->member = m_actor;
          m_command->operation = static_cast<PartyMemberOperation>(m_operation);
          m_command->initialize = m_isInit;
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

void Dialog_ChangePartyMember::drawPickers() {
  if (m_actorPicker) {
    if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
      if (confirmed) {
        m_actor = m_actorPicker->selection();
      }
      m_actorPicker.reset();
    }
  }
}