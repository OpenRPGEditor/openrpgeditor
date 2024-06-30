#include "Core/DatabaseEditor/DBGameConstantsTab.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Core/DatabaseEditor.hpp"
#include "Core/Project.hpp"
#include "Database/GameConstants.hpp"
void DBGameConstantsTab::drawVariableAliasModal() {
  if (m_selectedVariable != -1 &&
      ImGui::BeginPopupModal("##var_alias_edit", nullptr,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (!m_currentAlias) {
      m_currentAlias = m_constants->variables[m_selectedVariable];
    }
    ImGui::Text("Alias:");
    ImGui::InputText("##alias_input", &m_constants->variables[m_selectedVariable]);
    if (ImGui::Button("OK")) {
      m_currentAlias.reset();
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      m_constants->variables[m_selectedVariable] = *m_currentAlias;
      m_currentAlias.reset();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

void DBGameConstantsTab::drawSwitchAliasModal() {
  if (m_selectedSwitch != -1 && ImGui::BeginPopupModal("##switch_alias_edit", nullptr,
                                                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (!m_currentAlias) {
      m_currentAlias = m_constants->switches[m_selectedSwitch];
    }
    ImGui::Text("Alias:");
    ImGui::InputText("##alias_input", &m_constants->switches[m_selectedSwitch]);
    if (ImGui::Button("OK")) {
      m_currentAlias.reset();
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      m_constants->variables[m_selectedSwitch] = *m_currentAlias;
      m_currentAlias.reset();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}
void DBGameConstantsTab::draw() {
  ImGui::BeginChild("#orpg_constants_tab");
  ImGui::SeparatorText("Exported Constants");
  ImGui::Checkbox("Generate JS file", &m_constants->generateJS);
  ImGui::SameLine();
  if (ImGui::Button("Generate Now")) {
    if (m_constants->generateConstantsJS(m_parent->project()->database().basePath + "js/Constants.js")) {}
  }
  ImGui::SameLine();
  if (ImGui::Button("Save")) {
    m_constants->serialize(m_parent->project()->database().basePath + "data/Constants.json");
  }
  if (ImGui::BeginTabBar("##ore_constants_tabbar")) {
    if (ImGui::BeginTabItem("Variables")) {
      ImGui::Text("When exported to Constants.js all variables will be prefixed with VAR_");
      if (ImGui::BeginTable("##ore_variable_constants_table", 3,
                            ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                                ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Alias");
        ImGui::TableHeadersRow();

        for (auto& [id, alias] : m_constants->variables) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if (ImGui::Selectable(std::format("{:04}", id).c_str(), m_selectedVariable == id,
                                ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              ImGui::OpenPopup("##var_alias_edit");
            }
            m_selectedVariable = id;
          }
          if (ImGui::IsItemFocused()) {
            m_selectedVariable = id;
          }
          ImGui::TableNextColumn();
          ImGui::Text("%s", m_parent->variables(id).c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", alias.c_str());
        }
        drawVariableAliasModal();

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::Selectable("##ore_variable_constants_new_alias", false,
                              ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            m_picker.emplace("Variables", m_parent->project()->system().variables);
          }
          m_selectedVariable = -1;
        }

        if (m_picker) {
          const auto [closed, confirmed] = m_picker->draw();
          if (closed) {
            const int selection = m_picker->selection();
            if (confirmed && !m_constants->variables.contains(selection)) {
              m_constants->variables[selection] = std::format("VARIABLE_{:04}", selection);
            }
            m_picker.reset();
          }
        }
        ImGui::EndTable();
      }
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selectedVariable != -1) {
        m_constants->variables.erase(m_selectedVariable);
      }

      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Switches")) {
      ImGui::Text("When exported to Constants.js all switches will be prefixed with SW_");
      if (ImGui::BeginTable("##ore_switch_constants_table", 3,
                            ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                                ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Alias");
        ImGui::TableHeadersRow();

        for (auto& [id, alias] : m_constants->switches) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if (ImGui::Selectable(std::format("{:04}", id).c_str(), m_selectedSwitch == id,
                                ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              ImGui::OpenPopup("##switch_alias_edit");
            }
            m_selectedSwitch = id;
          }
          if (ImGui::IsItemFocused()) {
            m_selectedVariable = id;
          }
          drawSwitchAliasModal();
          ImGui::TableNextColumn();
          ImGui::Text("%s", m_parent->switches(id).c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", alias.c_str());
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        if (ImGui::Selectable("##ore_switch_constants_new_alias", false,
                              ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            m_picker.emplace("Switches", m_parent->project()->system().switches);
          }
          m_selectedSwitch = -1;
        }

        if (m_picker) {
          const auto [closed, confirmed] = m_picker->draw();
          if (closed) {
            const int selection = m_picker->selection();
            if (confirmed && !m_constants->switches.contains(selection)) {
              m_constants->switches[selection] = std::format("SWITCH_{:04}", selection);
            }
            m_picker.reset();
          }
        }
        ImGui::EndTable();
      }
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selectedSwitch != -1) {
        m_constants->variables.erase(m_selectedSwitch);
      }
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::EndChild();
}