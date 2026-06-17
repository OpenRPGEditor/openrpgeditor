#include "Editor/DatabaseEditor/DBGameConstantsTab.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "IconsFontAwesome6.h"

#include "Editor/ImGuiExt/ImGuiNotify.hpp"
static constexpr std::string_view kAliasPopupId = "###AliasEdit";

void DBGameConstantsTab::drawAliasModal(const GameConstants::Type type) {
  if (m_selection != -1 && ImGui::BeginPopupModal(std::format("{}{}", trNOOP("Constant Alias"), kAliasPopupId).c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    std::map<int, std::string>* map;
    std::string name;
    switch (type) {
    case GameConstants::Type::Switch:
      map = &m_constants->switches;
      name = Database::instance()->system->switche(m_selection);
      break;
    case GameConstants::Type::Variable:
      map = &m_constants->variables;
      name = Database::instance()->system->variable(m_selection);
      break;
    case GameConstants::Type::Actor:
      map = &m_constants->actors;
      name = Database::instance()->actors->actor(m_selection)->name();
      break;
    case GameConstants::Type::Class:
      map = &m_constants->classes;
      name = Database::instance()->classes->classType(m_selection)->name();
      break;
    case GameConstants::Type::Skill:
      map = &m_constants->skills;
      name = Database::instance()->skills->skill(m_selection)->name();
      break;
    case GameConstants::Type::Item:
      map = &m_constants->items;
      name = Database::instance()->items->item(m_selection)->name();
      break;
    case GameConstants::Type::Weapon:
      map = &m_constants->weapons;
      name = Database::instance()->weapons->weapon(m_selection)->name();
      break;
    case GameConstants::Type::Armor:
      map = &m_constants->armors;
      name = Database::instance()->armors->armor(m_selection)->name();
      break;
    case GameConstants::Type::Enemy:
      map = &m_constants->enemies;
      name = Database::instance()->enemies->enemy(m_selection)->name();
      break;
    case GameConstants::Type::Troop:
      map = &m_constants->troops;
      name = Database::instance()->troops->troop(m_selection)->name();
      break;
    case GameConstants::Type::State:
      map = &m_constants->states;
      name = Database::instance()->states->state(m_selection)->name();
      break;
    case GameConstants::Type::Animation:
      map = &m_constants->animations;
      name = Database::instance()->animations->animation(m_selection)->name();
      break;
    case GameConstants::Type::Tileset:
      map = &m_constants->tilesets;
      name = Database::instance()->tilesets->tileset(m_selection)->name();
      break;
    case GameConstants::Type::CommonEvent:
      map = &m_constants->commonEvents;
      name = Database::instance()->commonEvents->event(m_selection)->name();
      break;
    case GameConstants::Type::Map:
      map = &m_constants->maps;
      name = Database::instance()->mapInfos->map(m_selection)->name();
      break;
    default:
      return;
    }

    if (!m_currentAlias) {
      m_currentAlias = (*map)[m_selection];
    }

    ImGui::BeginVertical("##alias_popup_main_layout", {-1, -1}, 0.f);
    {
      const bool isValid = m_constants->isValidName(type, m_selection, m_currentAlias.value());
      ImGui::Text(trNOOP("Name: %s"), name.c_str());
      ImGui::BeginHorizontal("##alias_popup_alias_layout", {-1, 0});
      {
        if (!isValid) {
          ImGui::TextUnformatted(trNOOP("Alias: Selected alias is invalid!"));
          ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF0000FF);
          ImGui::PushStyleColor(ImGuiCol_ChildBg, 0xFF0000FF);
        } else {
          ImGui::TextUnformatted(trNOOP("Alias:"));
          ImGui::PushStyleColor(ImGuiCol_FrameBg, 0xFF007F00);
          ImGui::PushStyleColor(ImGuiCol_ChildBg, 0xFF007F00);
        }
        ImGui::InputText("##alias_input", &m_currentAlias.value());
        ImGui::PopStyleColor(2);
      }
      ImGui::EndHorizontal();
      ImGui::Separator();
      ImGui::BeginHorizontal("##alias_popup_buttons_layout", {-1, 0});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##alias_popup_buttons", {trNOOP("OK"), trNOOP("Cancel")}, false, {}, {!isValid}); ret == 0) {
          (*map)[m_selection] = *m_currentAlias;
          m_currentAlias.reset();
          ImGui::CloseCurrentPopup();
        } else if (ret == 1) {
          m_currentAlias.reset();
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }
}

void DBGameConstantsTab::setupTableHeaders() {
  ImGui::TableSetupColumn(trNOOP("ID"), ImGuiTableColumnFlags_WidthFixed);
  ImGui::TableSetupColumn(trNOOP("Name"), ImGuiTableColumnFlags_WidthFixed);
  ImGui::TableSetupColumn(trNOOP("Alias"), ImGuiTableColumnFlags_WidthStretch);
  ImGui::TableSetupColumn("##delete", ImGuiTableColumnFlags_WidthFixed);
  ImGui::TableSetupScrollFreeze(4, 1);
  ImGui::TableHeadersRow();
}

bool DBGameConstantsTab::drawSelectable(const int id, const bool selected) {
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
  ImGui::TextUnformatted(name.c_str());
  ImGui::TableNextColumn();
  ImGui::TextUnformatted(alias.c_str());
}

bool DBGameConstantsTab::drawDeleteButton(int id) {
  ImGui::TableNextColumn();
  if (ImGui::Selectable((std::string(ICON_FA_DELETE_LEFT) + std::format("##delete_btn{}", id)).c_str(), false, ImGuiSelectableFlags_AllowOverlap, {ImGui::GetFrameHeight(), ImGui::GetFrameHeight()})) {
    return true;
  }
  ImGui::SetItemTooltip("%s", trNOOP("Delete constant export"));
  return false;
}

bool DBGameConstantsTab::drawAddButton() { return ImGui::Button(trNOOP("Add"), {-1, 0}); }
void DBGameConstantsTab::draw() {
  GroupBox mainGroup(tr("Exported Constants"), "##constants_tab_group", {-1, -1});
  if (mainGroup.begin()) {
    ImGui::BeginVertical("##constants_tab_main_layout", {-1, -1}, 0.f);
    {
      ImGui::BeginHorizontal("##constants_generate_js_layout", {-1, -1}, 0.f);
      {
        ImGui::Checkbox(trNOOP("Generate JS file"), &m_constants->generateJS);
        if (ImGui::Button(trNOOP("Generate Now"))) {
          if (m_constants->generateConstantsJS(Database::instance()->basePath / "js/Constants.js")) {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Generated js/Constants.js successfully!"});
          } else {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize js/Constants.js!"});
          }
        }
        if (ImGui::Button(trNOOP("Save"))) {
          if (m_constants->serialize(Database::instance()->basePath / "data/Constants.json")) {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Success, "Serialized data/Constants.json successfully!"});
          } else {
            ImGui::InsertNotification(ImGuiToast{ImGuiToastType::Error, "Failed to serialize data/Constants.json!"});
          }
        }
      }
      ImGui::EndHorizontal();
      if (ImGui::BeginTabBar("##constants_tab_bar")) {
        if (ImGui::BeginTabItem(trNOOP("Variables"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Variables will be prefixed with VAR_"));
          if (ImGui::BeginTable("##constants_variables_tab", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();

            for (auto it = m_constants->variables.begin(); it != m_constants->variables.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->system->variable(id), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->variables.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_switchVariablePicker.emplace(VariableSwitchPicker::Type::Variable, Database::instance()->system->variables(), 1);
            m_switchVariablePicker->setOpen(true);
          }

          if (m_switchVariablePicker) {
            if (const auto [closed, confirmed] = m_switchVariablePicker->draw(); closed) {
              if (const int selection = m_switchVariablePicker->selection(); confirmed && !m_constants->variables.contains(selection)) {
                m_constants->variables[selection] = std::format("VARIABLE_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_switchVariablePicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }

          drawAliasModal(GameConstants::Type::Variable);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->variables.erase(m_selection);
          }

          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Switches"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Switches will be prefixed with SW_"));
          if (ImGui::BeginTable("##switch_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->switches.begin(); it != m_constants->switches.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->system->switche(id), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->switches.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }
          if (drawAddButton()) {
            m_switchVariablePicker.emplace(VariableSwitchPicker::Type::Switch, Database::instance()->system->switches(), 1);
            m_switchVariablePicker->setOpen(true);
          }
          if (m_switchVariablePicker) {
            if (const auto [closed, confirmed] = m_switchVariablePicker->draw(); closed) {
              if (const int selection = m_switchVariablePicker->selection(); confirmed && !m_constants->switches.contains(selection)) {
                m_constants->switches[selection] = std::format("SWITCH_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_switchVariablePicker.reset();
            }
          }
          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Switch);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->switches.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Actors"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Actors will be prefixed with ACT_"));
          if (ImGui::BeginTable("##actors_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->actors.begin(); it != m_constants->actors.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->actors->actor(id)->name(), alias);
              if (drawDeleteButton(id)) {
                m_constants->actors.erase(id);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }
          if (drawAddButton()) {
            m_actorsPicker.emplace(Database::instance()->actors->actorList(), m_selection);
            m_actorsPicker->setOpen(true);
          }
          if (m_actorsPicker) {
            if (const auto [closed, confirmed] = m_actorsPicker->draw(); closed) {
              if (const int selection = m_actorsPicker->selection(); confirmed && !m_constants->actors.contains(selection)) {
                m_constants->actors[selection] = std::format("ACTOR_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_actorsPicker.reset();
            }
          }
          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Actor);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->actors.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Classes"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Classes will be prefixed with CLS_"));
          if (ImGui::BeginTable("##classes_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->classes.begin(); it != m_constants->classes.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->classes->classType(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->classes.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_classesPicker.emplace(Database::instance()->classes->classes(), m_selection);
            m_classesPicker->setOpen(true);
          }
          if (m_classesPicker) {
            if (const auto [closed, confirmed] = m_classesPicker->draw(); closed) {
              if (const int selection = m_classesPicker->selection(); confirmed && !m_constants->classes.contains(selection)) {
                m_constants->classes[selection] = std::format("CLASS_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_classesPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Class);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->classes.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Skills")) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Skills will be prefixed with SKL_"));
          if (ImGui::BeginTable("##skills_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->skills.begin(); it != m_constants->skills.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->skills->skill(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->skills.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_skillsPicker.emplace(Database::instance()->skills->skills(), m_selection);
            m_skillsPicker->setOpen(true);
          }
          if (m_skillsPicker) {
            if (const auto [closed, confirmed] = m_skillsPicker->draw(); closed) {
              if (const int selection = m_skillsPicker->selection(); confirmed && !m_constants->skills.contains(selection)) {
                m_constants->skills[selection] = std::format("SKILL_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_skillsPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Skill);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->skills.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Items"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Items will be prefixed with ITM_"));
          if (ImGui::BeginTable("##items_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->items.begin(); it != m_constants->items.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->items->item(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->items.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_itemsPicker.emplace(Database::instance()->items->items(), m_selection);
            m_itemsPicker->setOpen(true);
          }
          if (m_itemsPicker) {
            if (const auto [closed, confirmed] = m_itemsPicker->draw(); closed) {
              if (const int selection = m_itemsPicker->selection(); confirmed && !m_constants->items.contains(selection)) {
                m_constants->items[selection] = std::format("ITEM_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_itemsPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Item);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->items.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Weapons"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Weapons will be prefixed with WPN_"));
          if (ImGui::BeginTable("##weapons_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->weapons.begin(); it != m_constants->weapons.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->weapons->weapon(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->weapons.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_weaponsPicker.emplace(Database::instance()->weapons->weapons(), m_selection);
            m_weaponsPicker->setOpen(true);
          }
          if (m_weaponsPicker) {
            if (const auto [closed, confirmed] = m_weaponsPicker->draw(); closed) {
              if (const int selection = m_weaponsPicker->selection(); confirmed && !m_constants->weapons.contains(selection)) {
                m_constants->weapons[selection] = std::format("WEAPON_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_weaponsPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Weapon);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->weapons.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Armors"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Armors will be prefixed with ARM_"));
          if (ImGui::BeginTable("##armors_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->armors.begin(); it != m_constants->armors.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->armors->armor(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->armors.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_armorsPicker.emplace(Database::instance()->armors->armors(), m_selection);
            m_armorsPicker->setOpen(true);
          }
          if (m_armorsPicker) {
            if (const auto [closed, confirmed] = m_armorsPicker->draw(); closed) {
              if (const int selection = m_armorsPicker->selection(); confirmed && !m_constants->armors.contains(selection)) {
                m_constants->armors[selection] = std::format("ARMOR_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_armorsPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Armor);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->armors.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Enemies"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Enemies will be prefixed with ENM_"));
          if (ImGui::BeginTable("##enemies_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->enemies.begin(); it != m_constants->enemies.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->enemies->enemy(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->enemies.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_enemiesPicker.emplace(Database::instance()->enemies->enemies(), m_selection);
            m_enemiesPicker->setOpen(true);
          }
          if (m_enemiesPicker) {
            if (const auto [closed, confirmed] = m_enemiesPicker->draw(); closed) {
              if (const int selection = m_enemiesPicker->selection(); confirmed && !m_constants->enemies.contains(selection)) {
                m_constants->enemies[selection] = std::format("ENEMY_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_enemiesPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Enemy);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->enemies.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Troops"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Troops will be prefixed with TRP_"));
          if (ImGui::BeginTable("##troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->troops.begin(); it != m_constants->troops.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->troops->troop(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->troops.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_troopsPicker.emplace(Database::instance()->troops->troops(), m_selection);
            m_troopsPicker->setOpen(true);
          }
          if (m_troopsPicker) {
            if (const auto [closed, confirmed] = m_troopsPicker->draw(); closed) {
              if (const int selection = m_troopsPicker->selection(); confirmed && !m_constants->troops.contains(selection)) {
                m_constants->troops[selection] = std::format("TROOP_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_troopsPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Troop);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->troops.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("States"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all States will be prefixed with STA_"));
          if (ImGui::BeginTable("##troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->states.begin(); it != m_constants->states.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->states->state(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->states.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_statesPicker.emplace(Database::instance()->states->states(), m_selection);
            m_statesPicker->setOpen(true);
          }
          if (m_statesPicker) {
            if (const auto [closed, confirmed] = m_statesPicker->draw(); closed) {
              if (const int selection = m_statesPicker->selection(); confirmed && !m_constants->states.contains(selection)) {
                m_constants->states[selection] = std::format("STATE_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_statesPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::State);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->states.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Animations"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Animations will be prefixed with ANM_"));
          if (ImGui::BeginTable("##troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->animations.begin(); it != m_constants->animations.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->animations->animation(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->animations.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_animationPicker.emplace(Database::instance()->animations->animations(), m_selection);
            m_animationPicker->setOpen(true);
          }
          if (m_animationPicker) {
            if (const auto [closed, confirmed] = m_animationPicker->draw(); closed) {
              if (const int selection = m_animationPicker->selection(); confirmed && !m_constants->animations.contains(selection)) {
                m_constants->animations[selection] = std::format("ANIMATION_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_animationPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Animation);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->animations.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Tilesets"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Tilesets will be prefixed with TLS_"));
          if (ImGui::BeginTable("##troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->tilesets.begin(); it != m_constants->tilesets.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->tilesets->tileset(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->tilesets.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_tilesetPicker.emplace(Database::instance()->tilesets->tilesets(), m_selection);
            m_tilesetPicker->setOpen(true);
          }
          if (m_tilesetPicker) {
            if (const auto [closed, confirmed] = m_tilesetPicker->draw(); closed) {
              if (const int selection = m_tilesetPicker->selection(); confirmed && !m_constants->tilesets.contains(selection)) {
                m_constants->tilesets[selection] = std::format("TILESET_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_tilesetPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::Tileset);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->tilesets.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Common Events"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Common Events will be prefixed with CMN_"));
          if (ImGui::BeginTable("##troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->commonEvents.begin(); it != m_constants->commonEvents.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->commonEvents->event(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->commonEvents.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_commonEventPicker.emplace(Database::instance()->commonEvents->events(), m_selection);
            m_commonEventPicker->setOpen(true);
          }
          if (m_commonEventPicker) {
            if (const auto [closed, confirmed] = m_commonEventPicker->draw(); closed) {
              if (const int selection = m_commonEventPicker->selection(); confirmed && !m_constants->commonEvents.contains(selection)) {
                m_constants->commonEvents[selection] = std::format("COMMON_EVENT_{:04}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_commonEventPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
            m_openPopup = false;
          }
          drawAliasModal(GameConstants::Type::CommonEvent);
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selection != -1) {
            m_constants->commonEvents.erase(m_selection);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(trNOOP("Maps"))) {
          ImGui::TextUnformatted(trNOOP("When exported to Constants.js all Maps will be prefixed with MAP_"));
          if (ImGui::BeginTable("##troops_constants_table", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                                {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()})) {
            setupTableHeaders();
            for (auto it = m_constants->maps.begin(); it != m_constants->maps.end();) {
              auto id = it->first;
              auto alias = it->second;
              if (drawSelectable(id, m_selection == id)) {
                m_selection = id;
              }
              drawNameAndAliasColumns(Database::instance()->mapInfos->map(id)->name(), alias);
              if (drawDeleteButton(id)) {
                it = m_constants->maps.erase(it);
              } else {
                ++it;
              }
            }
            ImGui::EndTable();
          }

          if (drawAddButton()) {
            m_mapsPicker.emplace(Database::instance()->mapInfos->mapInfos(), m_selection);
            m_mapsPicker->setOpen(true);
          }
          if (m_mapsPicker) {
            if (const auto [closed, confirmed] = m_mapsPicker->draw(); closed) {
              if (const int selection = m_mapsPicker->selection(); confirmed && !m_constants->maps.contains(selection)) {
                m_constants->maps[selection] = std::format("MAP_{:03}", selection);
                m_selection = selection;
                m_openPopup = true;
              }
              m_mapsPicker.reset();
            }
          }

          if (m_openPopup) {
            ImGui::OpenPopup(kAliasPopupId.data());
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
    }
    ImGui::EndVertical();
  }
  mainGroup.end();
}