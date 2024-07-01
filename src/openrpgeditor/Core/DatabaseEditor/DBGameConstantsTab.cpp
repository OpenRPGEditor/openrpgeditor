#include "Core/DatabaseEditor/DBGameConstantsTab.hpp"

#include "imgui.h"
#include "Core/ImGuiNotify.hpp"
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
    const bool isValid =
        m_constants->isValidName(GameConstants::Type::Variable, m_selectedVariable, m_currentAlias.value());
    if (!isValid) {
      ImGui::SameLine();
      ImGui::Text(" Selected alias is invalid!");
      ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF0000FF);
    } else {
      ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF007F00);
    }
    ImGui::Text("%s", m_parent->variables(m_selectedVariable).c_str());
    ImGui::InputText("##alias_input", &m_currentAlias.value());
    ImGui::PopStyleColor();
    ImGui::BeginDisabled(!isValid);
    if (ImGui::Button("OK")) {
      m_constants->variables[m_selectedVariable] = *m_currentAlias;
      m_currentAlias.reset();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
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
    const bool isValid =
        m_constants->isValidName(GameConstants::Type::Switch, m_selectedSwitch, m_currentAlias.value());
    if (!isValid) {
      ImGui::SameLine();
      ImGui::Text(" Selected alias is invalid!");
      ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF0000FF);
    } else {
      ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF007F00);
    }
    ImGui::Text("%s", m_parent->switches(m_selectedSwitch).c_str());
    ImGui::InputText("##alias_input", &m_currentAlias.value());
    ImGui::PopStyleColor();

    ImGui::BeginDisabled(!isValid);
    if (ImGui::Button("OK")) {
      m_constants->switches[m_selectedSwitch] = *m_currentAlias;
      m_currentAlias.reset();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
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
    if (m_constants->generateConstantsJS(m_parent->project()->database().basePath + "js/Constants.js")) {
      ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Generated js/Constants.js successfully!"});
    } else {
      ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize js/Constants.js!"});
    }
  }
  ImGui::SameLine();
  if (ImGui::Button("Save")) {
    if (m_constants->serialize(m_parent->project()->database().basePath + "data/Constants.json")) {
      ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Serialized data/Constants.json successfully!"});
    } else {
      ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize data/Constants.json!"});
    }
  }
  if (ImGui::BeginTabBar("##ore_constants_tabbar")) {
    if (ImGui::BeginTabItem("Variables")) {
      ImGui::Text("When exported to Constants.js all variables will be prefixed with VAR_");
      if (ImGui::BeginTable("##ore_variable_constants_table", 4,
                            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX |
                                ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) -
                                          ImGui::GetStyle().FramePadding.y})) {
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Alias", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("##delete", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupScrollFreeze(4, 1);
        ImGui::TableHeadersRow();

        for (auto& [id, alias] : m_constants->variables) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if (ImGui::Selectable(std::format("{:04}", id).c_str(), m_selectedVariable == id,
                                ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns |ImGuiSelectableFlags_AllowOverlap)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_openPopup = true;
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
          ImGui::TableNextColumn();
          if (ImGui::Selectable(ICON_FA_DELETE_LEFT, false, ImGuiSelectableFlags_AllowOverlap)) {
            m_constants->variables.erase(id);
          }
          if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Delete constant export");
          }
        }

        ImGui::EndTable();
      }
      if (ImGui::Button("Add")) {
        m_picker.emplace("Variables", m_parent->project()->system().variables);
      }

      if (m_picker) {
        const auto [closed, confirmed] = m_picker->draw();
        if (closed) {
          const int selection = m_picker->selection();
          if (confirmed && !m_constants->variables.contains(selection)) {
            m_constants->variables[selection] = std::format("VARIABLE_{:04}", selection);
            m_selectedVariable = selection;
            m_openPopup = true;
          }
          m_picker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##var_alias_edit");
        m_openPopup = false;
      }

      drawVariableAliasModal();
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selectedVariable != -1) {
        m_constants->variables.erase(m_selectedVariable);
      }

      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Switches")) {
      ImGui::Text("When exported to Constants.js all switches will be prefixed with SW_");
      if (ImGui::BeginTable("##ore_switch_constants_table", 4,
                            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX |
                                ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) -
                                          ImGui::GetStyle().FramePadding.y})) {
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Alias", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("##delete", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupScrollFreeze(4, 1);
        ImGui::TableHeadersRow();

        for (auto& [id, alias] : m_constants->switches) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if (ImGui::Selectable(std::format("{:04}", id).c_str(), m_selectedSwitch == id,
                                ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_openPopup = true;
            }
            m_selectedSwitch = id;
          }
          if (ImGui::IsItemFocused()) {
            m_selectedVariable = id;
          }
          ImGui::TableNextColumn();
          ImGui::Text("%s", m_parent->switches(id).c_str());
          ImGui::TableNextColumn();
          ImGui::Text("%s", alias.c_str());
          ImGui::TableNextColumn();
          if (ImGui::Selectable(ICON_FA_DELETE_LEFT, false, ImGuiSelectableFlags_AllowOverlap)) {
            m_constants->switches.erase(id);
          }
          if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Delete constant export");
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_picker.emplace("Variables", m_parent->project()->system().variables);
      }
      if (m_picker) {
        const auto [closed, confirmed] = m_picker->draw();
        if (closed) {
          const int selection = m_picker->selection();
          if (confirmed && !m_constants->switches.contains(selection)) {
            m_constants->switches[selection] = std::format("SWITCH_{:04}", selection);
            m_selectedSwitch = selection;
              m_openPopup = true;
          }
          m_picker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##var_alias_edit");
        m_openPopup = false;
      }

      drawSwitchAliasModal();

      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selectedSwitch != -1) {
        m_constants->switches.erase(m_selectedSwitch);
      }
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::EndChild();
}