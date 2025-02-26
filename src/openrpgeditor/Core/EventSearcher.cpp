#include "Core/EventSearcher.hpp"
#include "Core/MainWindow.hpp"
#include "Core/Settings.hpp"
#include "Database/EventCommands/MovementRoute/Script.hpp"
#include "ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
template <>
inline int ObjectPicker<std::optional<CommonEvent>>::getId(const std::optional<CommonEvent>& value) {
  return value ? value->id() : 0;
}

static const std::string InvalidCommonEvent = "Invalid Common Event";
template <>
inline const std::string& ObjectPicker<std::optional<CommonEvent>>::getName(const std::optional<CommonEvent>& value) {
  return value ? value->name() : InvalidCommonEvent;
}
bool EventSearcher::getListing() { return true; }

void EventSearcher::draw() {
  if (!m_isOpen) {
    return;
  }
  if (picker) {
    auto [closed, confirmed] = picker->draw();
    if (closed) {
      if (confirmed) {
        m_pickedData = picker->selection();
        m_valueSelected = false;
      }
      picker.reset();
    }
  }
  if (m_animationPicker) {
    if (m_animationPicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_animationPicker.reset();
    }
  }
  if (m_actorPicker) {
    if (m_actorPicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_actorPicker.reset();
    }
  }
  if (m_statePicker) {
    if (m_statePicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_statePicker.reset();
    }
  }
  if (m_classPicker) {
    if (m_classPicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_classPicker.reset();
    }
  }
  if (m_enemyPicker) {
    if (m_enemyPicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_enemyPicker.reset();
    }
  }
  if (m_itemPicker) {
    if (m_itemPicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_itemPicker.reset();
    }
  }
  if (m_armorPicker) {
    if (m_armorPicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_armorPicker.reset();
    }
  }
  if (m_weaponPicker) {
    if (m_weaponPicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_weaponPicker.reset();
    }
  }
  if (m_skillPicker) {
    if (m_skillPicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_skillPicker.reset();
    }
  }
  if (m_commonPicker) {
    if (m_commonPicker->drawPicker(m_pickedData)) {
      m_valueSelected = false;
      m_commonPicker.reset();
    }
  }

  if (ImGui::Begin("Event Searcher", &m_isOpen)) {
    type = static_cast<SearchType>(m_selectedSearchType);
    ImGui::Text("Search By:");
    ImGui::BeginGroup();
    {
      if (ImGui::BeginCombo("##orpg_search_combobox", DecodeEnumName(static_cast<SearchType>(m_selectedSearchType)).c_str())) {
        for (auto v : magic_enum::enum_values<SearchType>()) {
          if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<SearchType>(m_selectedSearchType) == v)) {
            m_selectedSearchType = static_cast<int>(v);
          }
        }
        ImGui::EndCombo();
      }
      if (type == SearchType::Variable) { // Variable
        if (ImGui::Button(Database::instance()->variableNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          picker.emplace("Variables", Database::instance()->system.variables(), m_selectedData);
          picker->setOpen(true);
        }
      } else if (type == SearchType::Switch) {
        // Switch
        if (ImGui::Button(Database::instance()->switchNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          picker.emplace("Switches", Database::instance()->system.switches(), m_selectedData);
          picker->setOpen(true);
        }
      } else if (type == SearchType::Animation) {
        if (ImGui::Button(Database::instance()->animationNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_animationPicker.emplace("Animations"sv, Database::instance()->animations.animations(), m_selectedData);
          m_animationPicker->setOpen(true);
        }
      } else if (type == SearchType::Armors) {
        if (ImGui::Button(Database::instance()->armorNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_armorPicker.emplace("Armors"sv, Database::instance()->armors.armors(), m_selectedData);
          m_armorPicker->setOpen(true);
        }
      } else if (type == SearchType::Audio) {
        ImGui::InputText("##orpg_search_textInput_audio", &m_pickedString);
      } else if (type == SearchType::CommonEvent) {
        if (ImGui::Button(Database::instance()->commonEventNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_commonPicker = ObjectPicker("Common Events"sv, Database::instance()->commonEvents.events(), m_selectedData);
          m_commonPicker->setOpen(true);
        }
      } else if (type == SearchType::Class) {
        if (ImGui::Button(Database::instance()->classNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_classPicker.emplace("Classes"sv, Database::instance()->classes.classes(), m_selectedData);
          m_classPicker->setOpen(true);
        }
      } else if (type == SearchType::Enemy) {
        if (ImGui::Button(Database::instance()->enemyNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_enemyPicker.emplace("Enemies"sv, Database::instance()->enemies.enemies(), m_selectedData);
          m_enemyPicker->setOpen(true);
        }
      } else if (type == SearchType::Items) {
        if (ImGui::Button(Database::instance()->itemNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_itemPicker.emplace("Items"sv, Database::instance()->items.items(), m_selectedData);
          m_itemPicker->setOpen(true);
        }
      } else if (type == SearchType::Skill) {
        if (ImGui::Button(Database::instance()->skillNameOrId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_skillPicker.emplace("Skills"sv, Database::instance()->skills.skills(), m_selectedData);
          m_skillPicker->setOpen(true);
        }
      } else if (type == SearchType::State) {
        if (ImGui::Button(Database::instance()->stateNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_statePicker.emplace("States"sv, Database::instance()->states.states(), m_selectedData);
          m_statePicker->setOpen(true);
        }
      } else if (type == SearchType::Weapons) {
        if (ImGui::Button(Database::instance()->weaponNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_weaponPicker.emplace("Weapons"sv, Database::instance()->weapons.weapons(), m_selectedData);
          m_weaponPicker->setOpen(true);
        }
      } else if (type == SearchType::Actor) {

        if (ImGui::Button(Database::instance()->actorNameAndId(m_valueSelected ? m_selectedData : m_pickedData).c_str(), ImVec2{290, 0})) {
          m_actorPicker.emplace("Actors"sv, Database::instance()->actors.actorList(), m_selectedData);
          m_actorPicker->setOpen(true);
        }
      } else if (type == SearchType::Script) {
        ImGui::InputText("##orpg_search_textInput_script", &m_pickedString);
      } else {
        ImGui::Text("Not implemented");
      }
      ImGui::EndGroup();
    }
    // TODO: Audio is giving weird results
    // TODO: Start working on reference mapping -- search results should be accurate.
    // TODO: Start with common event swaps first, maybe animations? This would be a good start
    // TODO: Database reorganization, favorites, animation preview, liblcf impl
    if (ImGui::Button("Search")) {
      m_valueSelected = true;
      if (type == SearchType::Script || type == SearchType::Audio) {
        m_searchString = m_pickedString;
        reference.findAllReferences(m_searchString, type);
      } else {
        m_selectedData = m_pickedData;
        reference.findAllReferences(m_selectedData, type);
      }
      m_maxPage = reference.totalSize() / TOTAL_ENTRIES;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 500);
    if (ImGui::ArrowButton("##orpg_searchpage_left", ImGuiDir_Left)) {
      if (m_currentPage > 0) {
        m_currentPage--;
      }
    }
    ImGui::SameLine();
    ImGui::Text(std::format("{}/{}", m_currentPage + 1, m_maxPage).c_str());
    ImGui::SameLine();
    if (ImGui::ArrowButton("##orpg_searchpage_right", ImGuiDir_Right)) {
      if (m_currentPage < m_maxPage - 1) {
        m_currentPage++;
      }
    }

    // Table with search results

    if (ImGui::BeginTable("##orpg_eventsearcher_eventlist", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                          ImVec2{0, ImGui::GetContentRegionAvail().y - 32 - ImGui::GetStyle().FramePadding.y})) {
      int tableIndex = 0;

      ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Map", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Page", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Step", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Tile", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupScrollFreeze(5, 0);
      ImGui::TableHeadersRow();
      ImGui::TableNextRow();

      totalEntries = 0;
      int resultBegin = TOTAL_ENTRIES * m_currentPage;
      for (auto& results : reference.getEvents()) {
        if (totalEntries < TOTAL_ENTRIES && resultBegin == 0) {
          const auto& event = results.getEvent();
          drawTable("Event Condition", results.getMapId(), event.id(), event.name(), event.x(), event.y(), results.getPage() + 1, results.getStep() + 1);
          totalEntries++;
        }
        if (resultBegin > 0) {

          resultBegin--;
        }
      }
      for (auto& results : reference.getCommands()) {
        if (totalEntries < TOTAL_ENTRIES && resultBegin == 0) {
          auto event = results.getEvent();

          if (type == SearchType::Script) {
            drawStringCommand(results.getCommand(), type, totalEntries, results.getStep() + 1);
            totalEntries++;
          }
          if (type == SearchType::CommonEvent || type == SearchType::Audio) {
            if (results.getCommand()->hasStringReference(m_searchString, type)) {
              drawTable("Command List", results.getMapId(), event.id(), event.name(), event.x(), event.y(), results.getPage() + 1, results.getStep() + 1);
              totalEntries++;
            }
          } else {
            if (results.getCommand()->hasReference(m_selectedData, type)) {
              drawTable("Command List", results.getMapId(), event.id(), event.name(), event.x(), event.y(), results.getPage() + 1, results.getStep() + 1);
              totalEntries++;
            }
          }
        }
        if (resultBegin > 0) {

          resultBegin--;
        }
      }
      for (auto& commonEv : reference.getCommons()) {
        if (totalEntries < TOTAL_ENTRIES && resultBegin == 0) {
          if (type == SearchType::Script) {
            CommonEvent* common = Database::instance()->commonEvents.event(commonEv.getCommonId());
            for (auto& commonCommands : common->commands()) {
              drawStringCommand(commonCommands, type, tableIndex, commonEv.getStep() + 1);
              totalEntries++;
            }
          } else {
            drawTable(commonEv.getCommonId(), totalEntries, commonEv.getStep() + 1);
            totalEntries++;
          }
          tableIndex++;
        }
        if (resultBegin > 0) {

          resultBegin--;
        }
      }
      ImGui::EndTable();
    }
  }
  ImGui::End();
}
void EventSearcher::drawTable(std::string label, int mapId, int eventId, std::string eventName, int x, int y, int pageNo, int step) {
  ImGui::TableNextColumn();
  const bool isSelected = (m_selectedEvent == eventId);

  if (ImGui::SelectableWithBorder(std::format("{}##orpg_table_event_type_entry{}_{}_{}_{}", label, mapId, eventId, pageNo, totalEntries).c_str(), isSelected,
                                  ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {

    if (isSelected)
      ImGui::SetItemDefaultFocus();
  }
  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_table_map_entry{}_{}", eventId, pageNo).c_str());
  ImGui::TextUnformatted(std::format("{:03}:{}", mapId, Database::instance()->mapName(mapId)).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_event_data_{}_{}", eventId, pageNo).c_str());
  ImGui::TextUnformatted(std::format("{:03}:{}", eventId, eventName).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_event_pageNo_{}_{}", eventId, pageNo).c_str());
  ImGui::TextUnformatted(std::to_string(pageNo).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_event_step_{}_{}", eventId, pageNo).c_str());
  ImGui::TextUnformatted(step == 0 ? "-" : std::to_string(step).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_event_pos_{}_{}", eventId, pageNo).c_str());
  ImGui::TextUnformatted(std::string(std::format("({},{})", x, y)).c_str());
  ImGui::PopID();
  ImGui::TableNextRow();
}
void EventSearcher::drawTable(int commonId, int tableIndex, int step) {
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
  ImGui::TextUnformatted(std::format("{:03}:{}", commonId, Database::instance()->commonEventName(commonId)).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_commonev_pageNo_{}", commonId).c_str());
  ImGui::Text("-");
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_commonev_step_{}", commonId).c_str());
  ImGui::TextUnformatted(step == 0 ? "-" : std::to_string(step).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_commonev_pos_{}", commonId).c_str());
  ImGui::Text("-");
  ImGui::PopID();
  ImGui::TableNextRow();
}
void EventSearcher::drawTable(std::string text, int tableIndex, int step) {
  const bool isSelected = (m_selectedEvent == tableIndex);

  ImGui::TableNextColumn();
  if (ImGui::SelectableWithBorder(std::format("{}##orpg_table_type_script_entry{}_{}", text, tableIndex, totalEntries).c_str(), isSelected,
                                  ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {

    if (isSelected)
      ImGui::SetItemDefaultFocus();
  }
  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_table_script_entry{}", tableIndex).c_str());
  ImGui::Text("-");
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_script_data_{}", tableIndex).c_str());
  ImGui::Text("-");
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_script_pageNo_{}", tableIndex).c_str());
  ImGui::Text("-");
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_script_step_{}", tableIndex).c_str());
  ImGui::TextUnformatted(step == 0 ? "-" : std::to_string(step).c_str());
  ImGui::PopID();

  ImGui::TableNextColumn();
  ImGui::PushID(std::format("##orpg_eventsearcher_script_pos_{}", tableIndex).c_str());
  ImGui::Text("-");
  ImGui::PopID();
  ImGui::TableNextRow();
}
void EventSearcher::drawStringCommand(std::shared_ptr<IEventCommand> command, SearchType type, int tableIndex, int step) {
  if (command->hasStringReference(m_searchString, type)) {
    if (command->code() == EventCode::Script) {
      const auto cmd = std::dynamic_pointer_cast<const ScriptCommand>(command);
      if (cmd->moreScript.size() > 0) {
        for (auto& more : cmd->moreScript) {
          drawTable(more->script, tableIndex, step);
        }
      }
      drawTable(cmd->script, tableIndex, step);
    } else if (command->code() == EventCode::Script_del_Movement) {
      auto cmd = std::dynamic_pointer_cast<const MovementScriptCommand>(command);
      drawTable(cmd->script, tableIndex, step);
    }
  }
}