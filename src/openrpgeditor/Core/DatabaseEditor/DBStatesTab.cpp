#include "Core/DatabaseEditor/DBStatesTab.hpp"
#include "Core/Application.hpp"
#include "Database/States.hpp"

#include "imgui.h"

DBStatesTab::DBStatesTab(States& States, DatabaseEditor* parent) : IDBEditorTab(parent), m_states(States) {
  m_selectedState = m_states.state(1);
  if (m_selectedState) {
    // m_traitsEditor.setTraits(&m_selectedClass->traits);
  }
}

void DBStatesTab::draw() {
  ImGui::BeginChild("#orpg_states_editor");
  {
    ImGui::BeginChild("##orpg_states_editor_states", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("States");
        ImGui::BeginChild("##orpg_states_editor_States_list", ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& state : m_states.states()) {
              if (state.id() == 0) {
                continue;
              }

              if (ImGui::Selectable(Database::instance()->stateNameAndId(state.id()).c_str(), &state == m_selectedState) || (ImGui::IsItemFocused() && m_selectedState != &state)) {
                m_selectedState = &state;
                // m_traitsEditor.setTraits(&m_selectedClass->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max States %i", m_states.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxStates = m_states.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_states_editor_states_state_properties");
    { ImGui::EndChild(); }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max States", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the state list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxStates);
        ImGui::SameLine();
        if (ImGui::Button("Accept")) {
          m_changeConfirmDialogOpen = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_changeIntDialogOpen = false;
        }
      }
      ImGui::End();

      if (m_changeConfirmDialogOpen) {
        if (ImGui::Begin("Confirm Change", &m_changeConfirmDialogOpen,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
          ImGui::Text("Are you sure?");
          if (ImGui::Button("Yes")) {
            const int tmpId = m_selectedState->id();
            m_states.resize(m_editMaxStates);
            m_selectedState = m_states.state(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_editMaxStates = m_states.count();
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
  }
}