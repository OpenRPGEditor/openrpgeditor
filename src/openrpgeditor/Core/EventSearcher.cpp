#include "Core/EventSearcher.hpp"
#include "Core/Settings.hpp"
#include "Core\MainWindow.hpp"
#include "ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"

bool EventSearcher::getListing() { return true; }

void EventSearcher::draw() {
  if (!m_isOpen) {
    return;
  }
  if (picker) {
    auto [closed, confirmed] = picker->draw();
    if (closed) {
      if (confirmed) {
        if (m_dataType) {
          m_selectedSwitch = picker->selection();
        } else {
          m_selectedVariable = picker->selection();
        }
      }
      picker.reset();
    }
  }
  if (ImGui::Begin("Event Searcher", &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Search By:");

    ImGui::BeginGroup();
    {
      ImGui::RadioButton("Switch", &m_selectedSearchType, 0);
      ImGui::RadioButton("Variable", &m_selectedSearchType, 1);
      ImGui::RadioButton("Event Name", &m_selectedSearchType, 2);
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::BeginDisabled(m_selectedSearchType != 0);
      if (ImGui::Button(m_selectedSearchType != 0 ? "##orpg_search_switch_empty" : Database::instance()->switchNameAndId(m_selectedSwitch).c_str(), ImVec2{250, 0})) {
        picker.emplace("Switches", Database::instance()->system.switches(), m_selectedSwitch);
        picker->setOpen(true);
        m_dataType = true;
      }
      ImGui::EndDisabled();
      ImGui::BeginDisabled(m_selectedSearchType != 1);
      if (ImGui::Button(m_selectedSearchType != 1 ? "##orpg_search_var_empty" : Database::instance()->variableNameAndId(m_selectedVariable).c_str(), ImVec2{250, 0})) {
        picker.emplace("Variables", Database::instance()->system.variables(), m_selectedVariable);
        picker->setOpen(true);
        m_dataType = false;
      }
      ImGui::EndDisabled();
      ImGui::BeginDisabled(m_selectedSearchType != 2);
      ImGui::InputText("##orpg_search_string", &m_searchString);
      ImGui::EndDisabled();
      ImGui::EndGroup();
    }
    // TODO - Common event search in command list
    // TODO - Control variable search in command list
    // TODO consider all iteration types to search for!
    if (ImGui::Button("Search")) {
      // Search button
      if (m_selectedSearchType == 0) {
        // Switch Search
        reference.findAllReferences(m_selectedSwitch, SearchType::Switch);
      } else if (m_selectedSearchType == 1) {
        // Variable Search
        reference.findAllReferences(m_selectedVariable, SearchType::Variable);
      } else if (m_selectedSearchType == 2) {
        // Event Name Search
        reference.findAllReferences(m_searchString, SearchType::EventName);
      }
    }
    // Table with search results

    if (ImGui::BeginTable("##orpg_eventsearcher_eventlist", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {

      ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Map", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Page", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Position", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupScrollFreeze(5, 0);
      ImGui::TableHeadersRow();
      ImGui::TableNextRow();

      int tableIndex{0};
      for (auto& pair : reference.getEvents()) {
        for (auto& eventRef : pair.second) {
          Event event = *std::dynamic_pointer_cast<const Event>(eventRef);
          int index{0};
          for (auto& page : event.pages()) {
            if (page.conditions().variableValid() && m_selectedSearchType == 1) {
              if (page.conditions().variableId() == m_selectedVariable) {
                drawTable("Event Condition", pair.first, event.id(), event.name(), event.x(), event.y(), index + 1);
                tableIndex++;
              }
            } else if ((page.conditions().switch1Valid() || page.conditions().switch2Valid()) && m_selectedSearchType == 0) {
              if (page.conditions().switch1Id() == m_selectedSwitch || page.conditions().switch2Id() == m_selectedSwitch) {
                drawTable("Event Condition", pair.first, event.id(), event.name(), event.x(), event.y(), index + 1);
                tableIndex++;
              }
            }
            index++;
          }
        }
      }
      for (auto& pair : reference.getCommands()) {
        for (auto& eventRef : pair.second) {
          Event event = *std::dynamic_pointer_cast<const Event>(eventRef);
          int index{0};
          for (auto& page : event.pages()) {
            bool resultFound{false};
            for (auto& commands : page.list()) {
              if (resultFound == false) { // We only want one entry per page -- so draw once if any result is found
                if (m_selectedSearchType == 0) {
                  if (commands->hasSwitch(m_selectedVariable)) {
                    drawTable("Command List", pair.first, event.id(), event.name(), event.x(), event.y(), index + 1);
                    resultFound = true;
                  }
                } else if (m_selectedSearchType == 1) {
                  if (commands->hasVariable(m_selectedVariable)) {
                    drawTable("Command List", pair.first, event.id(), event.name(), event.x(), event.y(), index + 1);
                    resultFound = true;
                  }
                }
              }
            }
            index++;
          }
        }
      }
      for (auto& commonEv : reference.getCommons()) {
        drawTable(commonEv, tableIndex);
      }
      ImGui::EndTable();
    }
  }
  ImGui::End();
}
void EventSearcher::drawTable(std::string label, int mapId, int eventId, std::string eventName, int x, int y, int pageNo) {

  ImGui::TableNextColumn();
  const bool isSelected = (m_selectedEvent == eventId);

  if (ImGui::SelectableWithBorder(std::format("{}##orpg_table_event_type_entry{}_{}_{}", label, mapId, eventId, pageNo).c_str(), isSelected,
                                  ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {

    if (isSelected)
      ImGui::SetItemDefaultFocus();
  }
  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_table_map_entry{}_{}", eventId, pageNo).c_str());
  ImGui::Text(std::format("{:03}:{}", mapId, Database::instance()->mapName(mapId)).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_event_data_{}_{}", eventId, pageNo).c_str());
  ImGui::Text(std::format("{:03}:{}", eventId, eventName).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_event_pageNo_{}_{}", eventId, pageNo).c_str());
  ImGui::Text(std::to_string(pageNo).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_event_pos_{}_{}", eventId, pageNo).c_str());
  ImGui::Text(std::string(std::format("({},{})", x, y)).c_str());
  ImGui::PopID();
  ImGui::TableNextRow();
}
void EventSearcher::drawTable(int commonId, int tableIndex) {
  const bool isSelected = (m_selectedEvent == tableIndex);

  ImGui::TableNextColumn();
  if (ImGui::SelectableWithBorder(std::format("{}##orpg_table_type_common_entry{}", "Common Event", commonId).c_str(), isSelected,
                                  ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {

    if (isSelected)
      ImGui::SetItemDefaultFocus();
  }
  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_table_commonev_entry{}", commonId).c_str());
  ImGui::Text("-");
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_commonev_data_{}", commonId).c_str());
  ImGui::Text(std::format("{:03}:{}", commonId, Database::instance()->commonEventName(commonId)).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_commonev_pageNo_{}", commonId).c_str());
  ImGui::Text("-");
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_commonev_pos_{}", commonId).c_str());
  ImGui::Text("-");
  ImGui::PopID();
  ImGui::TableNextRow();
}