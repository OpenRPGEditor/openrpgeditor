#include "Core/EventCommands/Dialog_ChangeState.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeState::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeState");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 12) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ChangeState", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_state_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##change_state_actor_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (actorGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_state_actor_group_horizontal", {-1, -1});
        {
          ImGui::BeginVertical("##change_state_actor_group_radios", {-1, -1});
          {
            ImGui::RadioButton(trNOOP("Fixed"), &m_comparison, 0);
            ImGui::RadioButton(trNOOP("Variable"), &m_comparison, 1);
          }
          ImGui::EndVertical();
          ImGui::Spring();
          ImGui::BeginVertical("##change_state_actor_group_buttons", {-1, -1});
          {
            ImGui::BeginDisabled(m_comparison != 0);
            ImGui::PushID("##change_state_actor_group_fixed_button");
            if (ImGui::Button(m_comparison == 0 ? Database::instance()->actorNameAndId(m_actor).c_str() : "", {-1, 0})) {
              m_actorPicker = ObjectPicker(trNOOP("Actor"), Database::instance()->actors.actorList(), m_actor);
              m_actorPicker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_comparison != 1);
            ImGui::PushID("##change_state_actor_group_var");
            if (ImGui::Button(m_comparison == 1 ? Database::instance()->variableNameAndId(m_var).c_str() : "", {-1, 0})) {
              m_variablePicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_var);
              m_variablePicker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();
          }
          ImGui::EndVertical();
        }
        ImGui::EndHorizontal();
      }
      actorGroupBox.end();
      GroupBox operationGroupBox(trNOOP("Operation"), "##change_state_operation_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (operationGroupBox.begin()) {
        ImGui::BeginHorizontal("##change_state_operation_group_horizontal", {ImGui::GetContentRegionAvail().x, -1});
        {
          ImGui::Spring(.5);
          ImGui::RadioButton(trNOOP("Add"), &m_stateOp, 0);
          ImGui::RadioButton(trNOOP("Remove"), &m_stateOp, 1);
          ImGui::Spring(.5);
        }
        ImGui::EndHorizontal();
      }
      operationGroupBox.end();
      GroupBox skillGroupBox(trNOOP("Skill"), "##change_state_state_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (skillGroupBox.begin()) {
        ImGui::PushID("##change_state_state_selection");
        if (ImGui::Button(Database::instance()->stateNameAndId(m_state).c_str(), ImVec2{-1, 0})) {
          m_statePicker = ObjectPicker(trNOOP("State"), Database::instance()->states.states(), m_state);
          m_statePicker->setOpen(true);
        }
        ImGui::PopID();
      }
      skillGroupBox.end();
      ImGui::Spring();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_mp_dialog_buttons", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_mp_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->comparison = static_cast<ActorComparisonSource>(m_comparison);
          m_command->state = m_state;
          m_command->stateOp = static_cast<PartyMemberOperation>(m_stateOp);
          if (m_command->comparison == ActorComparisonSource::Fixed) {
            m_command->value = m_actor;
          } else {
            m_command->value = m_var;
          }

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

void Dialog_ChangeState::drawPickers() {
  if (m_actorPicker) {
    if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
      if (confirmed) {
        m_actor = m_actorPicker->selection();
      }
      m_actorPicker.reset();
    }
  }
  if (m_statePicker) {
    if (const auto [closed, confirmed] = m_statePicker->draw(); closed) {
      if (confirmed) {
        m_state = m_statePicker->selection();
      }
      m_statePicker.reset();
    }
  }

  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        m_var = m_variablePicker->selection();
      }
      m_variablePicker.reset();
    }
  }
}