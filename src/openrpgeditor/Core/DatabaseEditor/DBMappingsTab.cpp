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
  int move_from = -1, move_to = -1;
  ImGui::BeginChild("##orpg_mappings_editor_tab");
  {
    if (ImGui::BeginTabBar("##orpg_mappings_editor_tabs")) {

      if (ImGui::BeginTabItem("Variable")) {
        drawVariables();
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Switches")) {
        drawSwitches();
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Common Events")) {
        drawCommons();
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
    ImGui::EndChild();
  }
}
void DBMappingsTab::drawCommons() {
  const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
  ImGui::BeginChild("##orpg_mapping_header_commons", ImVec2{ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y});
  {

    ImGui::BeginChild("##orpg_mapping_panel_left_commons", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& common : Database::instance()->commonEvents.events()) {
        if (index > 0) {
          if (ImGui::Selectable(std::format("{}##orpg_mappings_{}_commons", common.value().name(), index).c_str(), m_selectedCommon == index)) {
            m_selectedCommon = index;
          }
        }

        ImGuiDragDropFlags src_flags = 0;
        src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
        src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
        // src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip
        if (ImGui::BeginDragDropSource(src_flags)) {
          if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
            ImGui::Text("Moving");
          ImGui::SetDragDropPayload("CommonList", &index, sizeof(int));
          ImGui::EndDragDropSource();
        }
        index++;
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_mapping_switch_panel_right_commons", ImVec2{ImGui::GetContentRegionMax().x / 2 - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& common : Database::instance()->commonEvents.events()) {
        if (index > 0) {
          if (ImGui::Selectable(std::format("{}##orpg_mappings_{}_commons", common.value().name(), index).c_str(), m_selectedCommon == index)) {
            m_selectedCommon = index;
          }
        }
        if (ImGui::BeginDragDropTarget()) {
          ImGuiDragDropFlags target_flags = 0;
          // target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery; // Don't wait until the delivery (release mouse button on a target) to do something
          //  target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CommonList", target_flags)) {
            move_from = *(const int*)payload->Data;
            move_to = index;
            reference_from.findAllReferences(move_from, SearchType::CommonEvent);
            reference_to.findAllReferences(move_to, SearchType::CommonEvent);

            for (auto& cmd : reference_from.getCommands()) {
              cmd.getCommand()->setReference(move_from, -1, SearchType::CommonEvent);
            }
            for (auto& cmd : reference_from.getCommons()) {
              cmd.getCommand()->setReference(move_from, -1, SearchType::CommonEvent);
            }

            for (auto& cmd : reference_to.getCommands()) {
              cmd.getCommand()->setReference(move_to, move_from, SearchType::CommonEvent);
            }
            for (auto& cmd : reference_to.getCommons()) {
              cmd.getCommand()->setReference(move_to, move_from, SearchType::CommonEvent);
            }

            for (auto& cmd : reference_from.getCommands()) {
              cmd.getCommand()->setReference(-1, move_to, SearchType::CommonEvent);
            }
            for (auto& cmd : reference_from.getCommons()) {
              cmd.getCommand()->setReference(-1, move_to, SearchType::CommonEvent);
            }
            Database::instance()->commonEvents.swapReference(move_from, move_to);
          }
          ImGui::EndDragDropTarget();
        }
        index++;
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();
}
void DBMappingsTab::drawSwitches() {
  const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
  ImGui::BeginChild("##orpg_mapping2_header_switches",
                    ImVec2{ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - (calc.y * 3 + ImGui::GetStyle().ItemSpacing.y * 3 + ImGui::GetStyle().FramePadding.y)});
  {
    ImGui::BeginChild("##orpg_mapping2_header_panel_left_switches", ImVec2{ImGui::GetContentRegionAvail().x / 2, calc.y * 3 + ImGui::GetStyle().ItemSpacing.y},
                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
    {
      ImGui::SeparatorText("Switches");
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
      if (ImGui::InputText("##orpg_mapping2_switch_edit_string_switches_left", &m_switch_string)) {
        m_system.setVariable(m_selectedSwitch, m_switch_string);
      }
      ImGui::PopItemWidth();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_mapping2_header_panel_right_switches", ImVec2{ImGui::GetContentRegionMax().x / 2, calc.y * 3 + ImGui::GetStyle().ItemSpacing.y});
    {
      ImGui::SeparatorText("Switches");
      ImGui::PushItemWidth(ImGui::GetContentRegionMax().x - ImGui::GetStyle().FramePadding.x);
      if (ImGui::InputText("##orpg_mapping2_switch_edit_string_switches_right", &m_switch_string)) {
        m_system.setSwitch(m_selectedSwitch, m_switch_string);
      }
      ImGui::PopItemWidth();
    }
    ImGui::EndChild();
    ImGui::BeginChild("##orpg_mapping2_switches_panel_left_switches", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y});
    {

      int index{0};
      for (auto& variable : m_system.switches()) {
        if (index > 0) {
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_variable_{}_switches", variable, index).c_str(), m_selectedSwitch == index)) {
            m_selectedSwitch = index;
            m_switch_string = variable;
          }
        }

        ImGuiDragDropFlags src_flags = 0;
        src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
        src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
        // src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip
        if (ImGui::BeginDragDropSource(src_flags)) {
          if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
            ImGui::Text("Moving");
          ImGui::SetDragDropPayload("SwitchList", &index, sizeof(int));
          ImGui::EndDragDropSource();
        }
        index++;
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_mapping2_switch_panel_right_switches",
                      ImVec2{ImGui::GetContentRegionMax().x / 2 - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y});
    {

      int index{0};
      for (auto& sw : m_system.switches()) {
        if (index > 0) {
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_switch_{}_switches", sw, index).c_str(), m_selectedSwitch == index)) {
            m_selectedSwitch = index;
            m_switch_string = sw;
          }
        }
        if (ImGui::BeginDragDropTarget()) {
          ImGuiDragDropFlags target_flags = 0;
          // target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery; // Don't wait until the delivery (release mouse button on a target) to do something
          //  target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SwitchList", target_flags)) {
            move_from = *(const int*)payload->Data;
            move_to = index;
            Database::instance()->commonEvents.swapReference(move_from, move_to);
          }
          ImGui::EndDragDropTarget();
        }
        index++;
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();

  ImGui::BeginChild("##orpg_mappings2_footer_switches");
  {
    ImGui::BeginChild("##orpg_mapping2_footer_panel_left_switches", ImVec2{ImGui::GetContentRegionMax().x / 2, 0});
    {
      ImGui::SeparatorText(trFormat("Max Switches {}", m_system.switches().size() - 1).c_str());
      if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
        m_changeIntDialogOpen = true;
        m_editMaxSwitches = m_system.switches().size() - 1;
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_mapping2_footer_panel_right_switches", ImVec2{ImGui::GetContentRegionMax().x / 2 - 10, 0});
    {

      ImGui::SeparatorText(trFormat("Max Switches {}", m_system.switches().size() - 1).c_str());
      if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
        m_changeIntDialogOpen = true;
        m_editMaxVariables = m_system.variables().size() - 1;
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();
}
void DBMappingsTab::drawVariables() {
  const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
  ImGui::BeginChild("##orpg_mapping2_header_variables",
                    ImVec2{ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - (calc.y * 3 + ImGui::GetStyle().ItemSpacing.y * 3 + ImGui::GetStyle().FramePadding.y)});
  {
    ImGui::BeginChild("##orpg_mapping2_header_panel_left_variables", ImVec2{ImGui::GetContentRegionAvail().x / 2, calc.y * 3 + ImGui::GetStyle().ItemSpacing.y},
                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
    {
      ImGui::SeparatorText("Variables");
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
      if (ImGui::InputText("##orpg_mapping2_edit_string_variables_left", &m_variable_string)) {
        m_system.setVariable(m_selectedVariable, m_variable_string);
      }
      ImGui::PopItemWidth();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_mapping2_header_panel_variables_switches", ImVec2{ImGui::GetContentRegionMax().x / 2, calc.y * 3 + ImGui::GetStyle().ItemSpacing.y});
    {
      ImGui::SeparatorText("Variables");
      ImGui::PushItemWidth(ImGui::GetContentRegionMax().x - ImGui::GetStyle().FramePadding.x);
      if (ImGui::InputText("##orpg_mapping2_edit_string_variables_right", &m_variable_string)) {
        m_system.setVariable(m_selectedVariable, m_variable_string);
      }
      ImGui::PopItemWidth();
    }
    ImGui::EndChild();
    ImGui::BeginChild("##orpg_mapping2_panel_left_variables", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y});
    {

      int index{0};
      for (auto& variable : m_system.variables()) {
        if (index > 0) {
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_variable_{}_variables", variable, index).c_str(), m_selectedVariable == index)) {
            m_selectedVariable = index;
            m_variable_string = variable;
          }
        }

        ImGuiDragDropFlags src_flags = 0;
        src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
        src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
        // src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip
        if (ImGui::BeginDragDropSource(src_flags)) {
          if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
            ImGui::Text("Moving");
          ImGui::SetDragDropPayload("VariableList", &index, sizeof(int));
          ImGui::EndDragDropSource();
        }
        index++;
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_mapping2_panel_right_variables",
                      ImVec2{ImGui::GetContentRegionMax().x / 2 - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y});
    {

      int index{0};
      for (auto& sw : m_system.variables()) {
        if (index > 0) {
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_{}_variables", sw, index).c_str(), m_selectedVariable == index)) {
            m_selectedVariable = index;
            m_variable_string = sw;
          }
        }
        if (ImGui::BeginDragDropTarget()) {
          ImGuiDragDropFlags target_flags = 0;
          // target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery; // Don't wait until the delivery (release mouse button on a target) to do something
          //  target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("VariableList", target_flags)) {
            move_from = *(const int*)payload->Data;
            move_to = index;
          }
          ImGui::EndDragDropTarget();
        }
        index++;
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();

  ImGui::BeginChild("##orpg_mappings2_footer_variables");
  {
    ImGui::BeginChild("##orpg_mapping2_footer_panel_left_variables", ImVec2{ImGui::GetContentRegionMax().x / 2, 0});
    {
      ImGui::SeparatorText(trFormat("Max Variables {}", m_system.variables().size() - 1).c_str());
      if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
        m_changeIntDialogOpen = true;
        m_editMaxVariables = m_system.variables().size() - 1;
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_mapping2_footer_panel_right_variables", ImVec2{ImGui::GetContentRegionMax().x / 2 - 10, 0});
    {

      ImGui::SeparatorText(trFormat("Max Variables {}", m_system.variables().size() - 1).c_str());
      if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
        m_changeIntDialogOpen = true;
        m_editMaxVariables = m_system.variables().size() - 1;
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();
}
