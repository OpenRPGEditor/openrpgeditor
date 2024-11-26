#include "Core/DatabaseEditor/DBGameConstantsTab.hpp"
#include "Core/ImGuiExt/ImGuiNotify.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "Core/DatabaseEditor.hpp"
#include "Core/Project.hpp"

#include "Database/GameConstants.hpp"

template <>
inline int ObjectPicker<std::optional<CommonEvent>>::getId(const std::optional<CommonEvent>& value) {
  return value ? value->id : 0;
}

static const std::string InvalidCommonEvent = "Invalid Common Event";
template <>
inline const std::string& ObjectPicker<std::optional<CommonEvent>>::getName(const std::optional<CommonEvent>& value) {
  return value ? value->name : InvalidCommonEvent;
}

template <>
inline int ObjectPicker<std::optional<MapInfo>>::getId(const std::optional<MapInfo>& value) {
  return value ? value->id : 0;
}

static const std::string InvalidMap = "Invalid Map";
template <>
inline const std::string& ObjectPicker<std::optional<MapInfo>>::getName(const std::optional<MapInfo>& value) {
  return value ? value->name : InvalidMap;
}

void DBGameConstantsTab::drawAliasModal(const GameConstants::Type type) {
  if (m_selection != -1 && ImGui::BeginPopupModal("##alias_edit", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    std::map<int, std::string>* map;
    std::string name;
    switch (type) {
    case GameConstants::Type::Switch:
      map = &m_constants->switches;
      name = m_parent->switches(m_selection);
      break;
    case GameConstants::Type::Variable:
      map = &m_constants->variables;
      name = m_parent->variables(m_selection);
      break;
    case GameConstants::Type::Actor:
      map = &m_constants->actors;
      name = m_parent->actor(m_selection)->name;
      break;
    case GameConstants::Type::Class:
      map = &m_constants->classes;
      name = m_parent->classType(m_selection)->name;
      break;
    case GameConstants::Type::Skill:
      map = &m_constants->skills;
      name = m_parent->skill(m_selection)->name;
      break;
    case GameConstants::Type::Item:
      map = &m_constants->items;
      name = m_parent->item(m_selection)->name;
      break;
    case GameConstants::Type::Weapon:
      map = &m_constants->weapons;
      name = m_parent->weapon(m_selection)->name;
      break;
    case GameConstants::Type::Armor:
      map = &m_constants->armors;
      name = m_parent->armor(m_selection)->name;
      break;
    case GameConstants::Type::Enemy:
      map = &m_constants->enemies;
      name = m_parent->enemy(m_selection)->name;
      break;
    case GameConstants::Type::Troop:
      map = &m_constants->troops;
      name = m_parent->troop(m_selection)->name;
      break;
    case GameConstants::Type::State:
      map = &m_constants->states;
      name = m_parent->state(m_selection)->name;
      break;
    case GameConstants::Type::Animation:
      map = &m_constants->animations;
      name = m_parent->animation(m_selection)->name;
      break;
    case GameConstants::Type::Tileset:
      map = &m_constants->tilesets;
      name = m_parent->tileset(m_selection)->name;
      break;
    case GameConstants::Type::CommonEvent:
      map = &m_constants->commonEvents;
      name = m_parent->commonEvent(m_selection)->name;
      break;
    case GameConstants::Type::Map:
      map = &m_constants->maps;
      name = Database::instance()->mapInfos.map(m_selection)->name;
      break;
    default:
      return;
    }

    if (!m_currentAlias) {
      m_currentAlias = (*map)[m_selection];
    }

    ImGui::Text("Alias:");
    const bool isValid = m_constants->isValidName(type, m_selection, m_currentAlias.value());
    if (!isValid) {
      ImGui::SameLine();
      ImGui::Text(" Selected alias is invalid!");
      ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF0000FF);
    } else {
      ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF007F00);
    }
    ImGui::Text("%s", name.c_str());
    ImGui::InputText("##alias_input", &m_currentAlias.value());
    ImGui::PopStyleColor();
    ImGui::BeginDisabled(!isValid);
    if (ImGui::Button("OK")) {
      (*map)[m_selection] = *m_currentAlias;
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

void DBGameConstantsTab::setupTableHeaders() {
  ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
  ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
  ImGui::TableSetupColumn("Alias", ImGuiTableColumnFlags_WidthStretch);
  ImGui::TableSetupColumn("##delete", ImGuiTableColumnFlags_WidthFixed);
  ImGui::TableSetupScrollFreeze(4, 1);
  ImGui::TableHeadersRow();
}

bool DBGameConstantsTab::drawSelectable(int id, bool selected) {
  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  if (ImGui::Selectable(std::format("{:04}", id).c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap)) {
    if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
      m_openPopup = true;
    }
    return true;
  }
  if (ImGui::IsItemFocused()) {
    return true;
  }
  return false;
}

void DBGameConstantsTab::drawNameAndAliasColumns(const std::string& name, const std::string& alias) {
  ImGui::TableNextColumn();
  ImGui::Text("%s", name.c_str());
  ImGui::TableNextColumn();
  ImGui::Text("%s", alias.c_str());
}
bool DBGameConstantsTab::drawDeleteButton(int id) {
  ImGui::TableNextColumn();
  if (ImGui::Selectable((std::string(ICON_FA_DELETE_LEFT) + std::format("##delete_btn{}", id)).c_str(), false, ImGuiSelectableFlags_AllowOverlap)) {
    return true;
  }
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Delete constant export");
  }
  return false;
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
      ImGui::Text("When exported to Constants.js all Variables will be prefixed with VAR_");
      if (ImGui::BeginTable("##ore_variable_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->variables.begin(); it != m_constants->variables.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->variables(id), alias);
          if (drawDeleteButton(id)) {
            it = m_constants->variables.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_switchVariblePicker.emplace("Variables", m_parent->project()->system().variables, 1);
        m_switchVariblePicker->setOpen(true);
      }

      if (m_switchVariblePicker) {
        const auto [closed, confirmed] = m_switchVariblePicker->draw();
        if (closed) {
          const int selection = m_switchVariblePicker->selection();
          if (confirmed && !m_constants->variables.contains(selection)) {
            m_constants->variables[selection] = std::format("VARIABLE_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_switchVariblePicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }

      drawAliasModal(GameConstants::Type::Variable);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->variables.erase(m_selection);
      }

      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Switches")) {
      ImGui::Text("When exported to Constants.js all Switches will be prefixed with SW_");
      if (ImGui::BeginTable("##ore_switch_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->switches.begin(); it != m_constants->switches.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->switches(id), alias);
          if (drawDeleteButton(id)) {
            it = m_constants->switches.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }
      if (ImGui::Button("Add")) {
        m_switchVariblePicker.emplace("Switches", m_parent->project()->system().switches, 1);
        m_switchVariblePicker->setOpen(true);
      }
      if (m_switchVariblePicker) {
        const auto [closed, confirmed] = m_switchVariblePicker->draw();
        if (closed) {
          const int selection = m_switchVariblePicker->selection();
          if (confirmed && !m_constants->switches.contains(selection)) {
            m_constants->switches[selection] = std::format("SWITCH_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_switchVariblePicker.reset();
        }
      }
      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Switch);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->switches.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Actors")) {
      ImGui::Text("When exported to Constants.js all Actors will be prefixed with ACT_");
      if (ImGui::BeginTable("##ore_actors_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->actors.begin(); it != m_constants->actors.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->actor(id)->name, alias);
          if (drawDeleteButton(id)) {
            m_constants->actors.erase(id);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }
      if (ImGui::Button("Add")) {
        m_actorsPicker.emplace("Actors", m_parent->project()->actors().actorList(), m_selection);
        m_actorsPicker->setOpen(true);
      }
      if (m_actorsPicker) {
        const auto [closed, confirmed] = m_actorsPicker->draw();
        if (closed) {
          const int selection = m_actorsPicker->selection();
          if (confirmed && !m_constants->actors.contains(selection)) {
            m_constants->actors[selection] = std::format("ACTOR_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_actorsPicker.reset();
        }
      }
      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Actor);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->actors.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Classes")) {
      ImGui::Text("When exported to Constants.js all Classes will be prefixed with CLS_");
      if (ImGui::BeginTable("##ore_classes_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->classes.begin(); it != m_constants->classes.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->classType(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->classes.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_classesPicker.emplace("Classes", m_parent->classes().classes(), m_selection);
        m_classesPicker->setOpen(true);
      }
      if (m_classesPicker) {
        const auto [closed, confirmed] = m_classesPicker->draw();
        if (closed) {
          const int selection = m_classesPicker->selection();
          if (confirmed && !m_constants->classes.contains(selection)) {
            m_constants->classes[selection] = std::format("CLASS_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_classesPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Class);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->classes.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Skills")) {
      ImGui::Text("When exported to Constants.js all Skills will be prefixed with SKL_");
      if (ImGui::BeginTable("##ore_skills_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->skills.begin(); it != m_constants->skills.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->skill(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->skills.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_skillsPicker.emplace("Skills", m_parent->skills(), m_selection);
        m_skillsPicker->setOpen(true);
      }
      if (m_skillsPicker) {
        const auto [closed, confirmed] = m_skillsPicker->draw();
        if (closed) {
          const int selection = m_skillsPicker->selection();
          if (confirmed && !m_constants->skills.contains(selection)) {
            m_constants->skills[selection] = std::format("SKILL_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_skillsPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Skill);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->skills.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Items")) {
      ImGui::Text("When exported to Constants.js all Items will be prefixed with ITM_");
      if (ImGui::BeginTable("##ore_items_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->items.begin(); it != m_constants->items.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->item(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->items.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_itemsPicker.emplace("Items", m_parent->items(), m_selection);
        m_itemsPicker->setOpen(true);
      }
      if (m_itemsPicker) {
        const auto [closed, confirmed] = m_itemsPicker->draw();
        if (closed) {
          const int selection = m_itemsPicker->selection();
          if (confirmed && !m_constants->items.contains(selection)) {
            m_constants->items[selection] = std::format("ITEM_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_itemsPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Item);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->items.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Weapons")) {
      ImGui::Text("When exported to Constants.js all Weapons will be prefixed with WPN_");
      if (ImGui::BeginTable("##ore_weapons_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->weapons.begin(); it != m_constants->weapons.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->weapon(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->weapons.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_weaponsPicker.emplace("Weapons", m_parent->weapons(), m_selection);
        m_weaponsPicker->setOpen(true);
      }
      if (m_weaponsPicker) {
        const auto [closed, confirmed] = m_weaponsPicker->draw();
        if (closed) {
          const int selection = m_weaponsPicker->selection();
          if (confirmed && !m_constants->weapons.contains(selection)) {
            m_constants->weapons[selection] = std::format("WEAPON_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_weaponsPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Weapon);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->weapons.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Armors")) {
      ImGui::Text("When exported to Constants.js all Armors will be prefixed with ARM_");
      if (ImGui::BeginTable("##ore_armors_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->armors.begin(); it != m_constants->armors.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->armor(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->armors.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_armorsPicker.emplace("Armors", m_parent->armors(), m_selection);
        m_armorsPicker->setOpen(true);
      }
      if (m_armorsPicker) {
        const auto [closed, confirmed] = m_armorsPicker->draw();
        if (closed) {
          const int selection = m_armorsPicker->selection();
          if (confirmed && !m_constants->armors.contains(selection)) {
            m_constants->armors[selection] = std::format("ARMOR_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_armorsPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Armor);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->armors.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Enemies")) {
      ImGui::Text("When exported to Constants.js all Enemies will be prefixed with ENM_");
      if (ImGui::BeginTable("##ore_enemies_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->enemies.begin(); it != m_constants->enemies.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->enemy(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->enemies.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_enemiesPicker.emplace("Enemies", m_parent->enemies(), m_selection);
        m_enemiesPicker->setOpen(true);
      }
      if (m_enemiesPicker) {
        const auto [closed, confirmed] = m_enemiesPicker->draw();
        if (closed) {
          const int selection = m_enemiesPicker->selection();
          if (confirmed && !m_constants->enemies.contains(selection)) {
            m_constants->enemies[selection] = std::format("ENEMY_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_enemiesPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Enemy);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->enemies.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Troops")) {
      ImGui::Text("When exported to Constants.js all Troops will be prefixed with TRP_");
      if (ImGui::BeginTable("##ore_troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->troops.begin(); it != m_constants->troops.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->troop(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->troops.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_troopsPicker.emplace("Troops", m_parent->troops(), m_selection);
        m_troopsPicker->setOpen(true);
      }
      if (m_troopsPicker) {
        const auto [closed, confirmed] = m_troopsPicker->draw();
        if (closed) {
          const int selection = m_troopsPicker->selection();
          if (confirmed && !m_constants->troops.contains(selection)) {
            m_constants->troops[selection] = std::format("TROOP_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_troopsPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Troop);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->troops.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("States")) {
      ImGui::Text("When exported to Constants.js all States will be prefixed with STA_");
      if (ImGui::BeginTable("##ore_troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->states.begin(); it != m_constants->states.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->state(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->states.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_statesPicker.emplace("States", m_parent->states().states(), m_selection);
        m_statesPicker->setOpen(true);
      }
      if (m_statesPicker) {
        const auto [closed, confirmed] = m_statesPicker->draw();
        if (closed) {
          const int selection = m_statesPicker->selection();
          if (confirmed && !m_constants->states.contains(selection)) {
            m_constants->states[selection] = std::format("STATE_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_statesPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::State);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->states.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Animations")) {
      ImGui::Text("When exported to Constants.js all Animations will be prefixed with ANM_");
      if (ImGui::BeginTable("##ore_troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->animations.begin(); it != m_constants->animations.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->animation(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->animations.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_animationPicker.emplace("Animations", m_parent->animations(), m_selection);
        m_animationPicker->setOpen(true);
      }
      if (m_animationPicker) {
        const auto [closed, confirmed] = m_animationPicker->draw();
        if (closed) {
          const int selection = m_animationPicker->selection();
          if (confirmed && !m_constants->animations.contains(selection)) {
            m_constants->animations[selection] = std::format("ANIMATION_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_animationPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Animation);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->animations.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Tilesets")) {
      ImGui::Text("When exported to Constants.js all Tilesets will be prefixed with TLS_");
      if (ImGui::BeginTable("##ore_troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->tilesets.begin(); it != m_constants->tilesets.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->tileset(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->tilesets.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_tilesetPicker.emplace("Tilesets", m_parent->tilesets(), m_selection);
        m_tilesetPicker->setOpen(true);
      }
      if (m_tilesetPicker) {
        const auto [closed, confirmed] = m_tilesetPicker->draw();
        if (closed) {
          const int selection = m_tilesetPicker->selection();
          if (confirmed && !m_constants->tilesets.contains(selection)) {
            m_constants->tilesets[selection] = std::format("TILESET_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_tilesetPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Tileset);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->tilesets.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Common Events")) {
      ImGui::Text("When exported to Constants.js all Common Events will be prefixed with CMN_");
      if (ImGui::BeginTable("##ore_troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->commonEvents.begin(); it != m_constants->commonEvents.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(m_parent->commonEvent(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->commonEvents.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_commonEventPicker.emplace("Common Events", m_parent->commonEvents(), m_selection);
        m_commonEventPicker->setOpen(true);
      }
      if (m_commonEventPicker) {
        const auto [closed, confirmed] = m_commonEventPicker->draw();
        if (closed) {
          const int selection = m_commonEventPicker->selection();
          if (confirmed && !m_constants->commonEvents.contains(selection)) {
            m_constants->commonEvents[selection] = std::format("COMMON_EVENT_{:04}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_commonEventPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::CommonEvent);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->commonEvents.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Maps")) {
      ImGui::Text("When exported to Constants.js all Maps will be prefixed with MAP_");
      if (ImGui::BeginTable("##ore_troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                            ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) - ImGui::GetStyle().FramePadding.y})) {
        setupTableHeaders();
        for (auto it = m_constants->maps.begin(); it != m_constants->maps.end();) {
          auto id = it->first;
          auto alias = it->second;
          if (drawSelectable(id, m_selection == id)) {
            m_selection = id;
          }
          drawNameAndAliasColumns(Database::instance()->mapInfos.map(id)->name, alias);
          if (drawDeleteButton(id)) {
            it = m_constants->maps.erase(it);
          } else {
            ++it;
          }
        }
        ImGui::EndTable();
      }

      if (ImGui::Button("Add")) {
        m_mapsPicker.emplace("Maps", Database::instance()->mapInfos.mapInfos(), m_selection);
        m_mapsPicker->setOpen(true);
      }
      if (m_mapsPicker) {
        const auto [closed, confirmed] = m_mapsPicker->draw();
        if (closed) {
          const int selection = m_mapsPicker->selection();
          if (confirmed && !m_constants->maps.contains(selection)) {
            m_constants->maps[selection] = std::format("MAP_{:03}", selection);
            m_selection = selection;
            m_openPopup = true;
          }
          m_mapsPicker.reset();
        }
      }

      if (m_openPopup) {
        ImGui::OpenPopup("##alias_edit");
        m_openPopup = false;
      }
      drawAliasModal(GameConstants::Type::Map);
      if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
        m_constants->maps.erase(m_selection);
      }
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::EndChild();
}