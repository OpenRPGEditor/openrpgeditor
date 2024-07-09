#include "Dialog_ChangeState.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Project.hpp"
#include "Database/Database.hpp"

std::tuple<bool, bool> Dialog_ChangeState::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{254, 205} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize)) {

    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (confirmed) {
        m_actor_var = picker->selection();
        picker.reset();
      }
    }
        if (actor_picker) {
          auto [closed, confirmed] = actor_picker->draw();
          if (confirmed) {
            m_actor = actor_picker->selection();
            actor_picker.reset();
          }
        }
    if (state_picker) {
      auto [closed, confirmed] = state_picker->draw();
      if (confirmed) {
        m_state = state_picker->selection();
        state_picker.reset();
      }
    }

    ImGui::SeparatorText("Actor");
    ImGui::BeginGroup();
    {;
      ImGui::RadioButton("Fixed", &m_comparison, 0);
      ImGui::RadioButton("Variable", &m_comparison, 1);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_comparison != 0);
      ImGui::PushID("##changestate_actor");
      if (ImGui::Button(
              m_comparison != 0 ? "" : Database::Instance->actorNameOrId(m_actor).c_str(),
              {(App::DPIHandler::scale_value(160)), 0})) {
        actor_picker = ObjectPicker<Actor>("Actors"sv, Database::Instance->actors.actorList(), 0);
      }
      ImGui::PopID();
      ImGui::EndDisabled();

      ImGui::BeginDisabled(m_comparison != 1);
      ImGui::PushID("##changeenemyhp_quant_var");
      if (ImGui::Button(
              m_comparison == 1 ? Database::Instance->variableNameAndId(m_actor_var).c_str() : "",
              ImVec2{(App::DPIHandler::scale_value(160)), 0})) {
        picker.emplace("Variables", m_project->system().variables);
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
    if (ImGui::Button(
            Database::Instance->stateNameOrId(m_state).c_str(),
            {(App::DPIHandler::scale_value(160)), 0})) {
      state_picker = ObjectPicker<State>("States"sv, Database::Instance->states.states(), 0);
    }
    ImGui::PopID();

    if (ImGui::Button("OK")) {
      m_confirmed = true;

            command->comparison = static_cast<ActorComparisonSource>(m_comparison);
            command->stateOp = static_cast<PartyMemberOperation>(m_stateOp);
            command->state = m_state;

            if (command->comparison == ActorComparisonSource::Variable)
              command->value = m_actor_var;
            else
              command->value = m_actor;



      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
