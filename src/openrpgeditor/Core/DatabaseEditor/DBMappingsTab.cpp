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
      // if (ImGui::TabItemButton("E", ImGuiTabItemFlags_NoReorder)) {}
      ImGui::EndTabBar();
    }
    ImGui::EndChild();
  }
}
void DBMappingsTab::drawCommons() {
  const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
  ImGui::BeginChild("##orpg_mapping_header_commons", ImVec2{ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y});
  {
    ImGui::BeginChild("##orpg_mapping2_header_panel_left_commons", ImVec2{ImGui::GetContentRegionAvail().x, calc.y * 2 - (ImGui::GetStyle().ItemSpacing.y * 2)},
                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
      ImGui::BeginDisabled(m_selectedCommon == 0);
      if (ImGui::InputText("##orpg_mapping_commonevent_name", &m_common_string)) {
        Database::instance()->commonEvents.event(m_selectedCommon)->setName(m_common_string);
      }
      ImGui::EndDisabled();
      ImGui::PopItemWidth();
    }
    ImGui::EndChild();
    ImGui::BeginChild("##orpg_mapping_panel_left_commons", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& common : Database::instance()->commonEvents.events()) {
        if (index > 0) {
          bool isStyle{false};
          if (std::find(m_reference_left.begin(), m_reference_left.end(), index) != m_reference_left.end()) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            isStyle = true;
          }
          bool isFormatted{false};
          if (isUnicodeFormatting(common.value().name())) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            isFormatted = true;
          }

          if (ImGui::Selectable(std::format("{}##orpg_mappings_{}_commons", std::format("#{:04} {}", common->id(), common.value().name()), index).c_str(),
                                m_selectedCommon == index || isStyle || isFormatted)) {
            m_selectedCommon = index;
            m_common_string = common.value().name();
          }
          if (isStyle || isFormatted) {
            ImGui::PopStyleColor(3);
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

          bool isStyle{false};
          if (std::find(m_reference_right.begin(), m_reference_right.end(), index) != m_reference_right.end()) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 1.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.0f, 0.43f));
            isStyle = true;
          }
          bool isFormatted{false};
          if (isUnicodeFormatting(common.value().name())) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            isFormatted = true;
          }
          if (ImGui::Selectable(std::format("{}##orpg_mappings_{}_commons", std::format("#{:04} {}", common->id(), common.value().name()), index).c_str(),
                                m_selectedCommon == index || isStyle || isFormatted)) {
            m_selectedCommon = index;
            m_common_string = common.value().name();
          }
          if (isStyle || isFormatted) {
            ImGui::PopStyleColor(3);
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

            m_reference_left.push_back(move_from);
            m_reference_right.push_back(move_to);
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
  ImGui::BeginChild("##orpg_mapping2_header_switches", ImVec2{ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y * 11});
  {
    ImGui::BeginChild("##orpg_mapping2_header_panel_left_switches", ImVec2{ImGui::GetContentRegionAvail().x, calc.y * 2 - (ImGui::GetStyle().ItemSpacing.y * 2)},
                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    {
      ImGui::SetNextItemAllowOverlap();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
      ImVec2 test = ImGui::GetCursorPos();
      ImGui::BeginDisabled(m_selectedSwitch == 0);
      if (ImGui::InputText("##orpg_mapping_switch_name", &m_switch_string)) {
        Database::instance()->system.setSwitch(m_selectedSwitch, m_switch_string);
      }
      ImGui::SetCursorPos(test);
      ImGui::Button("test", ImVec2(50, 50));
      ImGui::EndDisabled();
      ImGui::PopItemWidth();
    }
    ImGui::EndChild();
    ImGui::BeginChild("##orpg_mapping2_switches_panel_left_switches", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& sw : m_system.switches()) {
        if (index > 0) {

          bool isStyle{false};
          if (std::find(m_reference_left.begin(), m_reference_left.end(), index) != m_reference_left.end()) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            isStyle = true;
          }
          bool isFormatted{false};
          if (isUnicodeFormatting(sw)) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            isFormatted = true;
          }

          if (ImGui::Selectable(std::format("{}##orpg_mappings2_{}_switches", sw, index).c_str(), m_selectedSwitch == index || isStyle || isFormatted)) {
            m_selectedSwitch = index;
            m_switch_string = sw;
          }
          if (isStyle || isFormatted) {
            ImGui::PopStyleColor(3);
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
    ImGui::BeginChild("##orpg_mapping2_switch_panel_right_switches", ImVec2{ImGui::GetContentRegionMax().x / 2 - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& sw : m_system.switches()) {
        if (index > 0) {
          bool isStyle{false};
          if (std::find(m_reference_left.begin(), m_reference_left.end(), index) != m_reference_left.end()) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            isStyle = true;
          }
          bool isFormatted{false};
          if (isUnicodeFormatting(sw)) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            isFormatted = true;
          }
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_switch_{}_switches", sw, index).c_str(), m_selectedSwitch == index || isStyle || isFormatted)) {
            m_selectedSwitch = index;
            m_switch_string = sw;
          }
          if (isStyle || isFormatted) {
            ImGui::PopStyleColor(3);
          }
        }
        if (ImGui::BeginDragDropTarget()) {
          ImGuiDragDropFlags target_flags = 0;
          // target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery; // Don't wait until the delivery (release mouse button on a target) to do something
          //  target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SwitchList", target_flags)) {
            move_from = *(const int*)payload->Data;
            move_to = index;
            Database::instance()->system.swapSwitchReference(move_from, move_to);
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
  ImGui::BeginChild("##orpg_mapping2_header_variables", ImVec2{ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y * 11});
  {
    ImGui::BeginChild("##orpg_mapping2_header_panel_left_variables", ImVec2{ImGui::GetContentRegionAvail().x, calc.y * 2 - (ImGui::GetStyle().ItemSpacing.y * 2)},
                      ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    {

      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
      ImGui::BeginDisabled(m_selectedSwitch == 0);
      if (ImGui::InputText("##orpg_mapping_switch_name", &m_switch_string)) {
        Database::instance()->system.setSwitch(m_selectedSwitch, m_switch_string);
      }
      ImGui::EndDisabled();
      ImGui::PopItemWidth();
    }
    ImGui::EndChild();
    ImGui::BeginChild("##orpg_mapping2_panel_left_variables", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& variable : m_system.variables()) {
        if (index > 0) {
          bool isStyle{false};
          if (std::find(m_reference_left.begin(), m_reference_left.end(), index) != m_reference_left.end()) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            isStyle = true;
          }
          bool isFormatted{false};
          if (isUnicodeFormatting(variable)) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            isFormatted = true;
          }
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_variable_{}_variables", variable, index).c_str(), m_selectedVariable == index || isStyle || isFormatted)) {
            m_selectedVariable = index;
            m_variable_string = variable;
          }
          if (isStyle || isFormatted) {
            ImGui::PopStyleColor(3);
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
    ImGui::BeginChild("##orpg_mapping2_panel_right_variables", ImVec2{ImGui::GetContentRegionMax().x / 2 - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& variable : m_system.variables()) {
        if (index > 0) {
          bool isStyle{false};
          if (std::find(m_reference_left.begin(), m_reference_left.end(), index) != m_reference_left.end()) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f, 0.0f, 0.0f, 0.43f));
            isStyle = true;
          }
          bool isFormatted{false};
          if (isUnicodeFormatting(variable)) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            isFormatted = true;
          }
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_{}_variables", variable, index).c_str(), m_selectedVariable == index || isStyle || isFormatted)) {
            m_selectedVariable = index;
            m_variable_string = variable;
          }
          if (isStyle || isFormatted) {
            ImGui::PopStyleColor(3);
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
bool DBMappingsTab::isUnicodeFormatting(const std::string& text) {
  if (text.contains("\u25bc")) {
    return true;
  }
  return false;
}
