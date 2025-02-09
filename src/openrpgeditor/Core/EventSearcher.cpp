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
    if (ImGui::Button("Search")) {
      // Search button
      if (m_selectedSearchType == 0) {
        // Switch Search
        for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
          if (mapInfo->map()) {
            for (auto& event : mapInfo->map()->events()) {
              if (event) {
                std::vector<std::shared_ptr<const IModifiable>> searchVector = event.value().hasSwitch(m_selectedVariable);
                for (const auto& search : searchVector) {
                  m_events[mapInfo->id()].push_back(search);
                }
              }
            }
          }
        }
      } else if (m_selectedSearchType == 1) {
        // Variable Search
        for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
          if (mapInfo->map()) {
            for (auto& event : mapInfo->map()->events()) {
              if (event) {
                std::vector<std::shared_ptr<const IModifiable>> searchVector = event.value().hasVariable(m_selectedVariable);
                for (const auto& search : searchVector) {
                  m_events[mapInfo->id()].push_back(search);
                }
              }
            }
          }
        }
      } else if (m_selectedSearchType == 2) {
        // Event Name Search
      }
    }
    // Table with search results

    if (ImGui::BeginTable("##orpg_eventsearcher_eventlist", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {

      ImGui::TableSetupColumn("Map", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Page", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Position", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupScrollFreeze(4, 0);
      ImGui::TableHeadersRow();
      ImGui::TableNextRow();

      for (auto& pair : m_events) {
        for (auto& eventRef : pair.second) {
          Event event = *std::dynamic_pointer_cast<const Event>(eventRef);
          int index{0};
          for (auto& page : event.pages()) {
            if (page.conditions().variableValid() && m_selectedSearchType == 1) {
              if (page.conditions().variableId() == m_selectedVariable) {
                drawTable(pair.first, event.id(), event.name(), event.x(), event.y(), index + 1);
              }
            } else if ((page.conditions().switch1Valid() || page.conditions().switch2Valid()) && m_selectedSearchType == 0) {
              if (page.conditions().switch1Id() == m_selectedSwitch || page.conditions().switch2Id() == m_selectedSwitch) {
                drawTable(pair.first, event.id(), event.name(), event.x(), event.y(), index + 1);
              }
            }
            index++;
          }
        }
      }
      ImGui::EndTable();
    }
  }
  ImGui::End();
}
void EventSearcher::drawTable(int mapId, int eventId, std::string eventName, int x, int y, int pageNo) {

  ImGui::TableNextColumn();
  const bool isSelected = (m_selectedEvent == eventId);

  if (ImGui::SelectableWithBorder(std::format("{:03}:{}##orpg_table_map_entry{}_{}", mapId, Database::instance()->mapName(mapId), eventId, pageNo).c_str(), isSelected,
                                  ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {

    if (isSelected)
      ImGui::SetItemDefaultFocus();
  }

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