#include "Core/EventSearcher.hpp"
#include "Core/Settings.hpp"
#include "Core\MainWindow.hpp"
#include "Database/EventCommands/MovementRoute/Script.hpp"
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
        m_pickedData = picker->selection();
      }
      picker.reset();
    }
  }
  if (m_animationPicker) {
    if (m_animationPicker->drawPicker(m_pickedData)) {
      m_animationPicker.reset();
    }
  }
  if (m_actorPicker) {
    if (m_actorPicker->drawPicker(m_pickedData)) {
      m_actorPicker.reset();
    }
  }
  // if (m_commonPicker) {
  //   if (m_commonPicker->drawPicker(m_pickedData)) {
  //     m_commonPicker.reset();
  //   }
  // }
  if (m_statePicker) {
    if (m_statePicker->drawPicker(m_pickedData)) {
      m_statePicker.reset();
    }
  }
  // if (m_audioPicker) {
  //   if (m_audioPicker->drawPicker(m_pickedData)) {
  //     m_audioPicker.reset();
  //   }
  // }
  if (m_classPicker) {
    if (m_classPicker->drawPicker(m_pickedData)) {
      m_classPicker.reset();
    }
  }
  if (m_enemyPicker) {
    if (m_enemyPicker->drawPicker(m_pickedData)) {
      m_enemyPicker.reset();
    }
  }
  if (m_itemPicker) {
    if (m_itemPicker->drawPicker(m_pickedData)) {
      m_itemPicker.reset();
    }
  }
  if (m_armorPicker) {
    if (m_armorPicker->drawPicker(m_pickedData)) {
      m_armorPicker.reset();
    }
  }
  if (m_weaponPicker) {
    if (m_weaponPicker->drawPicker(m_pickedData)) {
      m_weaponPicker.reset();
    }
  }
  if (m_skillPicker) {
    if (m_skillPicker->drawPicker(m_pickedData)) {
      m_skillPicker.reset();
    }
  }

  type = static_cast<SearchType>(m_selectedSearchType);
  if (ImGui::Begin("Event Searcher", &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
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
        if (ImGui::Button(Database::instance()->variableNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          picker.emplace("Variables", Database::instance()->system.variables(), m_selectedData);
          picker->setOpen(true);
        }
      } else if (type == SearchType::Switch) {
        // Switch
        if (ImGui::Button(Database::instance()->switchNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          picker.emplace("Switches", Database::instance()->system.switches(), m_selectedData);
          picker->setOpen(true);
        }
      } else if (type == SearchType::Animation) {
        if (ImGui::Button(Database::instance()->animationNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          m_animationPicker.emplace("Animations"sv, Database::instance()->animations.animations(), m_selectedData);
          m_animationPicker->setOpen(true);
        }
      } else if (type == SearchType::Armors) {
        if (ImGui::Button(Database::instance()->armorNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          m_armorPicker.emplace("Armors"sv, Database::instance()->armors.armors(), m_selectedData);
          m_armorPicker->setOpen(true);
        }
      } else if (type == SearchType::Audio) {
        if (ImGui::Button(Database::instance()->system.sounds().at(m_selectedData).name().c_str(), ImVec2{290, 0})) {
          // m_audioPicker.emplace("Sound Effects"sv, Database::instance()->system.sounds(), m_selectedData);
          // m_audioPicker->setOpen(true);
        }
      } else if (type == SearchType::CommonEvent) {
        if (ImGui::Button(Database::instance()->commonEventNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          // m_commonPicker.emplace("Common Events"sv, Database::instance()->commonEvents.events(), m_selectedData);
          // m_commonPicker->setOpen(true);
        }
      } else if (type == SearchType::Class) {
        if (ImGui::Button(Database::instance()->classNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          m_classPicker.emplace("Classes"sv, Database::instance()->classes.classes(), m_selectedData);
          m_classPicker->setOpen(true);
        }
      } else if (type == SearchType::Enemy) {
        if (ImGui::Button(Database::instance()->enemyNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          m_enemyPicker.emplace("Enemies"sv, Database::instance()->enemies.enemies(), m_selectedData);
          m_enemyPicker->setOpen(true);
        }
      } else if (type == SearchType::Items) {
        if (ImGui::Button(Database::instance()->itemNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          m_itemPicker.emplace("Items"sv, Database::instance()->items.items(), m_selectedData);
          m_itemPicker->setOpen(true);
        }
      } else if (type == SearchType::Skill) {
        if (ImGui::Button(Database::instance()->skillNameOrId(m_selectedData).c_str(), ImVec2{290, 0})) {
          m_skillPicker.emplace("Skills"sv, Database::instance()->skills.skills(), m_selectedData);
          m_skillPicker->setOpen(true);
        }
      } else if (type == SearchType::State) {
        if (ImGui::Button(Database::instance()->stateNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          m_statePicker.emplace("States"sv, Database::instance()->states.states(), m_selectedData);
          m_statePicker->setOpen(true);
        }
      } else if (type == SearchType::Weapons) {
        if (ImGui::Button(Database::instance()->weaponNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          m_weaponPicker.emplace("Weapons"sv, Database::instance()->weapons.weapons(), m_selectedData);
          m_weaponPicker->setOpen(true);
        }
      } else if (type == SearchType::Actor)
        if (ImGui::Button(Database::instance()->actorNameAndId(m_selectedData).c_str(), ImVec2{290, 0})) {
          m_actorPicker.emplace("Actors"sv, Database::instance()->actors.actorList(), m_selectedData);
          m_actorPicker->setOpen(true);
        } else if (type == SearchType::Script) {
          if (ImGui::InputText("##orpg_search_textInput", &m_pickedString)) {}
        } else {
          ImGui::Text("Not implemented");
        }
      ImGui::EndGroup();
    }
    if (ImGui::Button("Search")) {
      if (type == SearchType::Script) {
        m_searchString = m_pickedString;
        reference.findAllReferences(m_searchString, type);
      } else {
        m_selectedData = m_pickedData;
        reference.findAllReferences(m_selectedData, type);
      }
    }
    //  ImGui::SameLine();
    //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 500);
    //  if (ImGui::ArrowButton("##orpg_searchpage_left", ImGuiDir_Left)) {
    //    if (m_currentPage > 0) {
    //      m_currentPage--;
    //    }
    //  }
    //  ImGui::SameLine();
    //  ImGui::Text(std::format("{}/{}", m_currentPage, m_maxPage).c_str());
    //  ImGui::SameLine();
    //  if (ImGui::ArrowButton("##orpg_searchpage_right", ImGuiDir_Right)) {
    //    if (m_currentPage > 0) {
    //      m_currentPage++;
    //    }
    //  }

    // Table with search results

    if (ImGui::BeginTable("##orpg_eventsearcher_eventlist", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, ImVec2{400, 400})) {
      int tableIndex = 0;

      ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Map", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Page", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupColumn("Position", ImGuiTableColumnFlags_WidthFixed);
      ImGui::TableSetupScrollFreeze(5, 0);
      ImGui::TableHeadersRow();
      ImGui::TableNextRow();

      totalEntries = 0;
      for (auto& pair : reference.getEvents()) {
        for (auto& eventRef : pair.second) {
          auto event = std::dynamic_pointer_cast<const Event>(eventRef);
          int index{0};
          for (auto& page : event->pages()) {
            if (page.conditions().variableValid() && type == SearchType::Variable) {
              if (page.conditions().variableId() == m_selectedData) {
                drawTable("Event Condition", pair.first, event->id(), event->name(), event->x(), event->y(), index + 1);
              }
            } else if ((page.conditions().switch1Valid() || page.conditions().switch2Valid()) && type == SearchType::Switch) {
              if (page.conditions().switch1Id() == m_selectedData || page.conditions().switch2Id() == m_selectedData) {
                drawTable("Event Condition", pair.first, event->id(), event->name(), event->x(), event->y(), index + 1);
              }
            }
            index++;
          }
        }
      }
      for (auto& pair : reference.getCommands()) {
        for (auto& ev : pair.second) {
          auto event = std::dynamic_pointer_cast<const Event>(ev);

          int index{0};
          for (auto& page : event->pages()) {
            bool resultFound{false};
            for (auto& commands : page.list()) {
              if (resultFound == false) { // We only want one entry per page -- so draw once if any result is found
                if (static_cast<SearchType>(m_selectedSearchType) == SearchType::Script) {
                  drawStringCommand(commands, type, tableIndex);
                } else {
                  if (commands->hasReference(m_selectedData, type)) {
                    drawTable("Command List", pair.first, event->id(), event->name(), event->x(), event->y(), index + 1);
                    resultFound = true;
                  }
                }
              }
            }
            index++;
          }
          tableIndex++;
        }
      }

      for (auto& commonEv : reference.getCommons()) {
        if (type == SearchType::Script) {
          CommonEvent* common = Database::instance()->commonEvents.event(commonEv);
          for (auto& commonCommands : common->commands()) {
            drawStringCommand(commonCommands, type, tableIndex);
          }
        } else {
          drawTable(commonEv, totalEntries);
        }
        tableIndex++;
      }
      ImGui::EndTable();
    }
  }
  ImGui::End();
}
void EventSearcher::drawTable(std::string label, int mapId, int eventId, std::string eventName, int x, int y, int pageNo) {

  totalEntries++;
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
  totalEntries++;
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
void EventSearcher::drawTable(std::string text, int tableIndex) {
  totalEntries++;
  const bool isSelected = (m_selectedEvent == tableIndex);

  ImGui::TableNextColumn();
  if (ImGui::SelectableWithBorder(std::format("{}##orpg_table_type_script_entry{}", text, tableIndex).c_str(), isSelected,
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
  ImGui::PushID(std::format("##orpg_eventsearcher_script_pos_{}", tableIndex).c_str());
  ImGui::Text("-");
  ImGui::PopID();
  ImGui::TableNextRow();
}
void EventSearcher::drawStringCommand(std::shared_ptr<IEventCommand> command, SearchType type, int tableIndex) {
  if (command->hasStringReference(m_searchString, type)) {
    if (command->code() == EventCode::Script) {
      auto cmd = std::dynamic_pointer_cast<const ScriptCommand>(command);
      if (cmd->moreScript.size() > 0) {
        for (auto& more : cmd->moreScript) {
          drawTable(more->script, tableIndex);
        }
      }
      drawTable(cmd->script, tableIndex);
    } else if (command->code() == EventCode::Script_del_Movement) {
      auto cmd = std::dynamic_pointer_cast<const MovementScriptCommand>(command);
      drawTable(cmd->script, tableIndex);
    }
  }
}