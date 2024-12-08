#include "Core/EventCommands/Dialog_ChangeState.hpp"

#include "Core/DPIHandler.hpp"
#include "Database/Database.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_ChangeState::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{254, 205} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    if (m_picker) {
      auto [closed, confirmed] = m_picker->draw();
      if (closed) {
        if (confirmed) {
          m_actorVar = m_picker->selection();
        }
        m_picker.reset();
      }
    }
    if (m_actorPicker) {
      auto [closed, confirmed] = m_actorPicker->draw();
      if (closed) {
        if (confirmed) {
          m_actor = m_actorPicker->selection();
        }
        m_actorPicker.reset();
      }
    }
    if (m_statePicker) {
      auto [closed, confirmed] = m_statePicker->draw();
      if (closed) {
        if (confirmed) {
          m_state = m_statePicker->selection();
        }
        m_statePicker.reset();
      }
    }

    ImGui::SeparatorText("Actor");
    ImGui::BeginGroup();
    {
      ;
      ImGui::RadioButton("Fixed", &m_comparison, 0);
      ImGui::RadioButton("Variable", &m_comparison, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_comparison != 0);
      ImGui::PushID("##changestate_actor");
      if (ImGui::Button(m_comparison != 0 ? "" : Database::instance()->actorNameOrId(m_actor).c_str(), ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        m_actorPicker = ObjectPicker<Actor>("Actors"sv, Database::instance()->actors.actorList(), m_actor);
        m_actorPicker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_comparison != 1);
      ImGui::PushID("##changeenemyhp_quant_var");
      if (ImGui::Button(m_comparison == 1 ? Database::instance()->variableNameAndId(m_actorVar).c_str() : "", ImVec2{200 - (15 * App::DPIHandler::get_ui_scale()), 0})) {
        m_picker.emplace("Variables", Database::instance()->system.variables, m_actorVar);
        m_picker->setOpen(true);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::EndGroup();
    }
    ImGui::SeparatorText("Operation");
    ImGui::RadioButton("Add", &m_stateOp, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Remove", &m_stateOp, 1);

    ImGui::SeparatorText("State");
    ImGui::PushID("##changestate_state");
    if (ImGui::Button(Database::instance()->stateNameOrId(m_state).c_str(), {(App::DPIHandler::scale_value(160)), 0})) {
      m_statePicker = ObjectPicker<State>("States"sv, Database::instance()->states.states(), m_state);
      m_statePicker->setOpen(true);
    }
    ImGui::PopID();

    if (ImGui::Button("OK")) {
      m_confirmed = true;

      m_command->comparison = static_cast<ActorComparisonSource>(m_comparison);
      m_command->stateOp = static_cast<PartyMemberOperation>(m_stateOp);
      m_command->state = m_state;

      if (m_command->comparison == ActorComparisonSource::Variable)
        m_command->value = m_actorVar;
      else
        m_command->value = m_actor;

      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
