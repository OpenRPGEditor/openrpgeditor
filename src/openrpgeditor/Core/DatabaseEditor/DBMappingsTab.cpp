#include "Core/DatabaseEditor/DBMappingsTab.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Skills.hpp"

#include "imgui.h"
DBMappingsTab::DBMappingsTab(DatabaseEditor* parent)
: IDBEditorTab(parent) {}

void DBMappingsTab::draw() {

  const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
  ImGui::BeginChild("##orpg_mappings_editor_tab");
  {
    if (ImGui::BeginTabBar("##orpg_mappings_editor_tabs")) {

      if (ImGui::BeginTabItem("Variable")) {
        if (m_selectedTab != 0) {
          if (!m_reference_left.empty()) {
            m_reference_left.clear();
            m_reference_right.clear();
          }
          m_selectedTab = 0;
        }

        drawVariables();
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Switches")) {
        if (m_selectedTab != 1) {
          if (!m_reference_left.empty()) {
            m_reference_left.clear();
            m_reference_right.clear();
          }
          m_selectedTab = 1;
        }
        drawSwitches();
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Common Events")) {
        if (m_selectedTab != 2) {
          if (!m_reference_left.empty()) {
            m_reference_left.clear();
            m_reference_right.clear();
          }
          m_selectedTab = 2;
        }
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
      ImGui::SetNextItemAllowOverlap();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
      ImGui::BeginDisabled(m_selectedCommon == 0);
      float pos = ImGui::GetCursorPosY();
      if (ImGui::InputText("##orpg_mapping_commonevent_name", &m_common_string)) {
        Database::instance()->commonEvents->event(m_selectedCommon)->setName(m_common_string);
      }
      ImGui::SetCursorPos(ImVec2{ImGui::GetContentRegionAvail().x - (ImGui::GetStyle().FramePadding.x * 8) + 4, pos + 4.f});
      ImGui::TextDisabled("(?)");
      if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(GetFormattingHelpText().data());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
      }
      ImGui::SameLine();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
      if (ImGui::Button(std::format("{}##{}", m_unicodes.at(m_selectedUnicode), "_commonevent").c_str(), ImVec2(30, 30))) {
        m_switch_string += m_unicodes.at(m_selectedUnicode);
        Database::instance()->commonEvents->event(m_selectedCommon)->setName(m_common_string);
      }
      if (ImGui::IsItemHovered()) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
          ImGui::OpenPopup("unicodePopup");
        }
      }
      if (ImGui::BeginPopup("unicodePopup")) {
        ImGui::SeparatorText("Unicodes");
        for (int i = 0; i < m_unicodes.size(); i++) {
          if (ImGui::Selectable(m_unicodes.at(i).data())) {
            m_selectedUnicode = i;
          }
        }
        ImGui::EndPopup();
      }
      ImGui::EndDisabled();
      ImGui::PopItemWidth();
    }
    ImGui::EndChild();
    ImGui::BeginChild("##orpg_mapping_panel_left_commons", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& common : Database::instance()->commonEvents->events()) {
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
      for (auto& common : Database::instance()->commonEvents->events()) {
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
            int move_from = *static_cast<const int*>(payload->Data);
            int move_to = index;
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
            Database::instance()->commonEvents->swapReference(move_from, move_to);
            Database::instance()->gameConstants->swap(move_from, move_to, Database::instance()->gameConstants->commonEvents);
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
      const float pos = ImGui::GetCursorPosY();
      ImGui::BeginDisabled(m_selectedSwitch == 0);
      if (ImGui::InputText("##orpg_mapping_switch_name", &m_switch_string)) {
        Database::instance()->system->setSwitch(m_selectedSwitch, m_switch_string);
      }
      ImGui::SetCursorPos(ImVec2{ImGui::GetContentRegionAvail().x - (ImGui::GetStyle().FramePadding.x * 8) + 4, pos + 4.f});
      ImGui::TextDisabled("(?)");
      if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(GetFormattingHelpText().data());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
      }
      ImGui::SameLine();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
      if (ImGui::Button(std::format("{}##{}", m_unicodes.at(m_selectedUnicode), "_switch").c_str(), ImVec2(30, 30))) {
        m_switch_string += m_unicodes.at(m_selectedUnicode);
        Database::instance()->system->setSwitch(m_selectedSwitch, m_switch_string);
      }
      if (ImGui::IsItemHovered()) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
          ImGui::OpenPopup("unicodePopup");
        }
      }
      ImGui::EndDisabled();
      ImGui::PopItemWidth();

      if (ImGui::BeginPopup("unicodePopup")) {
        ImGui::SeparatorText("Unicodes");
        for (int i = 0; i < m_unicodes.size(); i++) {
          if (ImGui::Selectable(m_unicodes.at(i).data())) {
            m_selectedUnicode = i;
          }
        }
        ImGui::EndPopup();
      }
    }
    ImGui::EndChild();
    ImGui::BeginChild("##orpg_mapping2_switches_panel_left_switches", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& sw : m_system->switches()) {
        if (index > 0) {

          bool isStyle{false};
          if (std::ranges::find(m_reference_left, index) != m_reference_left.end()) {
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

          if (ImGui::Selectable(std::format("{}##orpg_mappings2_{}_switches", std::format("#{:04} {}", index, sw), index).c_str(), m_selectedSwitch == index || isStyle || isFormatted)) {
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
      for (auto& sw : m_system->switches()) {
        if (index > 0) {
          bool isStyle{false};
          if (std::ranges::find(m_reference_right, index) != m_reference_right.end()) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 1.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.0f, 0.43f));
            isStyle = true;
          }
          bool isFormatted{false};
          if (isUnicodeFormatting(sw)) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            isFormatted = true;
          }
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_switch_{}_switches", std::format("#{:04} {}", index, sw), index).c_str(), m_selectedSwitch == index || isStyle || isFormatted)) {
            m_selectedSwitch = index;
            m_switch_string = sw;
          }
          if (isStyle || isFormatted) {
            ImGui::PopStyleColor(3);
          }
        }
        if (ImGui::BeginDragDropTarget()) {
          ImGuiDragDropFlags target_flags = 0;
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SwitchList", target_flags)) {
            int move_from = *static_cast<const int*>(payload->Data);
            int move_to = index;

            reference_from.findAllReferences(move_from, SearchType::Switch);
            reference_to.findAllReferences(move_to, SearchType::Switch);

            m_reference_left.push_back(move_from);
            m_reference_right.push_back(move_to);

            // Reference 1 => Temp Id
            for (auto& ev : reference_from.getEvents()) {
              ev.getEvent().setEventReference(ev.getPage(), move_from, -1, SearchType::Switch);
            }
            for (auto& cmd : reference_from.getCommands()) {
              cmd.getCommand()->setReference(move_from, -1, SearchType::Switch);
            }
            for (auto& cmd : reference_from.getCommons()) {
              if (cmd.getStep() == -2) {
                Database::instance()->commonEvents->event(cmd.getCommonEventId())->setReference(move_from, -1, SearchType::Switch);
              } else {
                cmd.getCommand()->setReference(move_from, -1, SearchType::Switch);
              }
            }

            // Reference 2 => Reference 1
            for (auto& ev : reference_from.getEvents()) {
              ev.getEvent().setEventReference(ev.getPage(), move_to, move_from, SearchType::Switch);
            }

            for (auto& cmd : reference_to.getCommands()) {
              cmd.getCommand()->setReference(move_to, move_from, SearchType::Switch);
            }
            for (auto& cmd : reference_to.getCommons()) {
              if (cmd.getStep() == -2) {
                Database::instance()->commonEvents->event(cmd.getCommonEventId())->setReference(move_to, move_from, SearchType::Switch);
              } else {
                cmd.getCommand()->setReference(move_to, move_from, SearchType::Switch);
              }
            }

            // Reference 1 => Reference 2
            for (auto& ev : reference_from.getEvents()) {
              ev.getEvent().setEventReference(ev.getPage(), -1, move_to, SearchType::Switch);
            }

            for (auto& cmd : reference_from.getCommands()) {
              cmd.getCommand()->setReference(-1, move_to, SearchType::Switch);
            }
            for (auto& cmd : reference_from.getCommons()) {
              if (cmd.getStep() == -2) {
                Database::instance()->commonEvents->event(cmd.getCommonEventId())->setReference(-1, move_to, SearchType::Switch);
              } else {
                cmd.getCommand()->setReference(-1, move_to, SearchType::Switch);
              }
            }

            Database::instance()->system->swapSwitchReference(move_from, move_to);
            Database::instance()->gameConstants->swap(move_from, move_to, Database::instance()->gameConstants->switches);
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
      ImGui::SeparatorText(trFormat("Max Switches {}", m_system->switches().size() - 1).c_str());
      if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
        m_changeIntDialogOpen = true;
        m_editMaxSwitches = m_system->switches().size() - 1;
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_mapping2_footer_panel_right_switches", ImVec2{ImGui::GetContentRegionMax().x / 2 - 10, 0});
    {

      ImGui::SeparatorText(trFormat("Max Switches {}", m_system->switches().size() - 1).c_str());
      if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
        m_changeIntDialogOpen = true;
        m_editMaxVariables = m_system->variables().size() - 1;
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

      ImGui::SetNextItemAllowOverlap();
      ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
      float pos = ImGui::GetCursorPosX();
      ImGui::BeginDisabled(m_selectedVariable == 0);
      if (ImGui::InputText("##orpg_mapping_variable_name", &m_variable_string)) {
        Database::instance()->system->setVariable(m_selectedVariable, m_variable_string);
      }
      ImGui::SetCursorPos(ImVec2{ImGui::GetContentRegionAvail().x - (ImGui::GetStyle().FramePadding.x * 8) + 4, pos + 4.f});
      ImGui::TextDisabled("(?)");
      if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(GetFormattingHelpText().data());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
      }
      ImGui::SameLine();
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4.f);
      if (ImGui::Button(std::format("{}##{}", m_unicodes.at(m_selectedUnicode), "_variable").c_str(), ImVec2(30, 30))) {
        m_variable_string += m_unicodes.at(m_selectedUnicode);
        Database::instance()->system->setSwitch(m_selectedVariable, m_variable_string);
      }
      if (ImGui::IsItemHovered()) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
          ImGui::OpenPopup("unicodePopup");
        }
      }
      ImGui::EndDisabled();
      ImGui::PopItemWidth();

      if (ImGui::BeginPopup("unicodePopup")) {
        ImGui::SeparatorText("Unicodes");
        for (int i = 0; i < m_unicodes.size(); i++) {
          if (ImGui::Selectable(m_unicodes.at(i).data())) {
            m_selectedUnicode = i;
          }
        }
        ImGui::EndPopup();
      }
    }
    ImGui::EndChild();
    ImGui::BeginChild("##orpg_mapping2_panel_left_variables", ImVec2{ImGui::GetContentRegionMax().x / 2, ImGui::GetContentRegionAvail().y});
    {

      int index{0};
      for (auto& variable : m_system->variables()) {
        if (index > 0) {
          bool isStyle{false};
          if (std::ranges::find(m_reference_left, index) != m_reference_left.end()) {
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
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_variable_{}_variables", std::format("#{:04} {}", index, variable), index).c_str(),
                                m_selectedVariable == index || isStyle || isFormatted)) {
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
      for (auto& variable : m_system->variables()) {
        if (index > 0) {
          bool isStyle{false};
          if (std::find(m_reference_right.begin(), m_reference_right.end(), index) != m_reference_right.end()) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f, 1.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.43f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 1.0f, 0.0f, 0.43f));
            isStyle = true;
          }
          bool isFormatted{false};
          if (isUnicodeFormatting(variable)) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.110f, 0.153f, 0.173f, 1.0f));
            isFormatted = true;
          }
          if (ImGui::Selectable(std::format("{}##orpg_mappings2_{}_variables", std::format("#{:04} {}", index, variable), index).c_str(), m_selectedVariable == index || isStyle || isFormatted)) {
            m_selectedVariable = index;
            m_variable_string = variable;
          }
          if (isStyle || isFormatted) {
            ImGui::PopStyleColor(3);
          }
        }
        if (ImGui::BeginDragDropTarget()) {
          ImGuiDragDropFlags target_flags = 0;
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("VariableList", target_flags)) {
            int move_from = *static_cast<const int*>(payload->Data);
            int move_to = index;
            reference_from.findAllReferences(move_from, SearchType::Variable);
            reference_to.findAllReferences(move_to, SearchType::Variable);

            m_reference_left.push_back(move_from);
            m_reference_right.push_back(move_to);

            for (auto& cmd : reference_from.getCommands()) {
              cmd.getCommand()->setReference(move_from, -1, SearchType::Variable);
            }
            for (auto& cmd : reference_from.getCommons()) {
              cmd.getCommand()->setReference(move_from, -1, SearchType::Variable);
            }

            for (auto& cmd : reference_to.getCommands()) {
              cmd.getCommand()->setReference(move_to, move_from, SearchType::Variable);
            }
            for (auto& cmd : reference_to.getCommons()) {
              cmd.getCommand()->setReference(move_to, move_from, SearchType::Variable);
            }

            for (auto& cmd : reference_from.getCommands()) {
              cmd.getCommand()->setReference(-1, move_to, SearchType::Variable);
            }
            for (auto& cmd : reference_from.getCommons()) {
              cmd.getCommand()->setReference(-1, move_to, SearchType::Variable);
            }

            Database::instance()->system->swapVariableReference(move_from, move_to);
            Database::instance()->gameConstants->swap(move_from, move_to, Database::instance()->gameConstants->variables);
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
      ImGui::SeparatorText(trFormat("Max Variables {}", m_system->variables().size() - 1).c_str());
      if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
        m_changeIntDialogOpen = true;
        m_editMaxVariables = m_system->variables().size() - 1;
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_mapping2_footer_panel_right_variables", ImVec2{ImGui::GetContentRegionMax().x / 2 - 10, 0});
    {

      ImGui::SeparatorText(trFormat("Max Variables {}", m_system->variables().size() - 1).c_str());
      if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
        m_changeIntDialogOpen = true;
        m_editMaxVariables = m_system->variables().size() - 1;
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
