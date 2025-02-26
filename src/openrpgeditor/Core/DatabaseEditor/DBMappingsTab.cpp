#include "Core/DatabaseEditor/DBMappingsTab.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Skills.hpp"

#include "imgui.h"
DBMappingsTab::DBMappingsTab(System& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_system(system) {
  m_switch_string = m_system.switche(m_selectedSwitch);
  m_variable_string = m_system.variable(m_selectedVariable);
}

void DBMappingsTab::draw() {

  const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
  ImGui::BeginChild("##orpg_mappings_editor_tab");
  {
    if (ImGui::BeginTabBar("##orpg_mappings_editor_tabs")) {

      if (ImGui::BeginTabItem("Assignment")) {

        ImGui::BeginChild("##orpg_mapping_header_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 60});
        {
          ImGui::SeparatorText("Variables");
          ImGui::PushItemWidth(ImGui::GetContentRegionMax().x - 20);
          if (ImGui::InputText("##orpg_mapping_variable_edit_string", &m_variable_string)) {
            m_system.setVariable(m_selectedVariable, m_variable_string);
          }
          ImGui::PopItemWidth();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_mapping_header_panel_right", ImVec2{ImGui::GetContentRegionMax().x / 2, 60});
        {
          ImGui::SeparatorText("Switches");
          ImGui::PushItemWidth(ImGui::GetContentRegionMax().x - 20);
          if (ImGui::InputText("##orpg_mapping_switch_edit_string", &m_switch_string)) {
            m_system.setSwitch(m_selectedSwitch, m_switch_string);
          }
          ImGui::PopItemWidth();
        }
        ImGui::EndChild();
        ImGui::BeginChild("##orpg_mapping_variable_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionMax().y - 170.f});
        {

          int index{0};
          for (auto& variable : m_system.variables()) {
            if (index > 0) {
              if (ImGui::Selectable(std::format("{}##orpg_mappings_variable_{}", variable, index).c_str(), m_selectedVariable == index || (ImGui::IsItemFocused() && m_selectedVariable != index))) {
                m_selectedVariable = index;
                m_variable_string = variable;
              }
            }
            index++;
          }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_mapping_switch_panel_right", ImVec2{ImGui::GetContentRegionMax().x / 2 - 10, ImGui::GetContentRegionMax().y - 170.f});
        {

          int index{0};
          for (auto& sw : m_system.switches()) {
            if (index > 0) {
              if (ImGui::Selectable(std::format("{}##orpg_mappings_switch_{}", sw, index).c_str(), m_selectedSwitch == index || (ImGui::IsItemFocused() && m_selectedSwitch != index))) {
                m_selectedSwitch = index;
                m_switch_string = sw;
              }
            }
            index++;
          }
        }
        ImGui::EndChild();
        ImGui::BeginChild("##orpg_mapping_footer_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 60});
        {
          ImGui::SeparatorText(trFormat("Max Switches {}", m_system.switches().size() - 1).c_str());
          if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
            m_changeIntDialogOpen = true;
            m_editMaxSwitches = m_system.switches().size() - 1;
          }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_mapping_footer_panel_right", ImVec2{ImGui::GetContentRegionMax().x / 2 - 10, 60});
        {

          ImGui::SeparatorText(trFormat("Max Variables {}", m_system.switches().size() - 1).c_str());
          if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
            m_changeIntDialogOpen = true;
            m_editMaxVariables = m_system.variables().size() - 1;
          }
        }
        ImGui::EndChild();

        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Mapping")) {
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
    ImGui::EndChild();
  }
}
