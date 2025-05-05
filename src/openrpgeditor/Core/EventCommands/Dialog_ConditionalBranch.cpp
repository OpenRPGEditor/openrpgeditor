#include "Core/EventCommands/Dialog_ConditionalBranch.hpp"

#include "Core/CommonUI/TextEditor.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"

#include "Database/Database.hpp"

#include "imgui.h"
#include <imgui_internal.h>

std::tuple<bool, bool> Dialog_ConditionalBranch::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ConditionalBranch");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 58) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 18) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ConditionalBranch", m_name).c_str(), &m_open)) {
    drawPickers();
    ImGui::BeginVertical("##conditional_branch_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      if (ImGui::BeginTabBar("##conditional_branch_dialog")) {
        ImGui::BeginVertical("##conditional_branch_main_layout_tabs", {-1, ImGui::GetContentRegionAvail().y * .7f}, 0);
        {
          drawDataTab();
          drawActorTab();
          drawEntityTab();
          drawMiscTab();
        }
        ImGui::EndVertical();
        ImGui::EndTabBar();
      }
      drawDialogButtons();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}

void Dialog_ConditionalBranch::drawDataTab() {
  if (ImGui::BeginTabItem(trNOOP("Data"), nullptr, m_selectedTab == 0 ? ImGuiTabItemFlags_SetSelected : 0)) {
    ImGui::BeginHorizontal("##conditional_branch_data_tab_main_layout", {-1, -1}, 0);
    {
      ImGui::BeginVertical("##conditional_branch_data_tab_left", {-1, 0}, 0);
      {
        ImGui::RadioButton(trNOOP("Switch"), &m_conditionType, 0);
        ImGui::RadioButton(trNOOP("Variable"), &m_conditionType, 1);
        // Constant
        ImGui::Dummy(ImVec2{-1, ImGui::GetFrameHeight()});
        // Fixed
        ImGui::Dummy(ImVec2{-1, ImGui::GetFrameHeight()});
        ImGui::RadioButton(trNOOP("Self Switch"), &m_conditionType, 2);
        ImGui::RadioButton(trNOOP("Timer"), &m_conditionType, 3);
      }
      ImGui::EndVertical();
      ImGui::BeginVertical("##conditional_branch_data_tab_right", {-1, 0}, 0);
      {
        ImGui::BeginHorizontal("##conditional_branch_data_tab_right_switch", {-1, 0}, 0);
        {
          ImGui::BeginDisabled(m_conditionType != 0);
          // Switches
          if (ImGui::EllipsesButton((m_conditionType != 0 ? "##conditional_switch_empty" : Database::instance()->switchNameAndId(m_switchId)).c_str(),
                                    ImVec2{ImGui::GetContentRegionAvail().x * .7f, 0})) {
            m_pickerType = 0;
            m_variablePicker.emplace(trNOOP("Switches"), Database::instance()->system.switches(), m_switchId);
            m_variablePicker->setOpen(true);
          }
          ImGui::TextUnformatted(trNOOP("is"));
          ImGui::PushItemWidth(-1);
          if (ImGui::BeginCombo("##conditional_switch_sub_selection", DecodeEnumName(magic_enum::enum_value<ValueControl>(m_switchValue)).c_str())) {
            for (auto& val : magic_enum::enum_values<ValueControl>()) {
              if (const bool isSelected = m_switchValue == magic_enum::enum_index(val).value(); ImGui::Selectable(DecodeEnumName(val).c_str(), isSelected)) {
                m_switchValue = static_cast<int>(val);
                if (isSelected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::EndDisabled();
        }
        ImGui::EndHorizontal();
        ImGui::BeginVertical("##conditional_branch_data_tab_right_variable", {-1, 0}, 0);
        {
          // Variables
          ImGui::BeginDisabled(m_conditionType != 1);
          {
            ImGui::BeginHorizontal("##conditional_branch_data_tab_right_variable_selection", {-1, 0}, 0);
            {
              if (ImGui::EllipsesButton((m_conditionType != 1 ? "##conditional_variable_id" : Database::instance()->variableNameAndId(m_variableId)).c_str(),
                                        ImVec2{ImGui::GetContentRegionAvail().x * .7f, 0})) {
                m_pickerType = 1;
                m_variablePicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_variableId);
                m_variablePicker->setOpen(true);
              }

              ImGui::PushItemWidth(-1);
              if (ImGui::BeginCombo("##conditional_variable_sub_selection", DecodeEnumName(magic_enum::enum_value<VariableComparisonType>(m_variableValue)).c_str())) {
                for (auto& val : magic_enum::enum_values<VariableComparisonType>()) {
                  if (const bool isSelected = m_variableValue == static_cast<int>(val); ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(val)).c_str(), isSelected)) {
                    m_variableValue = static_cast<int>(val);
                    if (isSelected) {
                      ImGui::SetItemDefaultFocus();
                    }
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndHorizontal();
            ImGui::BeginHorizontal("##conditional_branch_data_tab_right_variable_value", {-1, 0}, 0);
            {
              ImGui::BeginVertical("##conditional_branch_data_tab_right_variable_value_radios", {-1, 0}, 0);
              {
                ImGui::RadioButton(trNOOP("Constant"), &m_variableSubSource, 0);
                ImGui::RadioButton(trNOOP("Fixed"), &m_variableSubSource, 1);
              }
              ImGui::EndVertical();
              ImGui::BeginVertical("##conditional_branch_data_tab_right_variable_value_inputs", {-1, 0}, 0);
              {
                ImGui::BeginDisabled(m_variableSubSource != 0);
                ImGui::SetNextItemWidth(-1);
                if (ImGui::SpinInt("##conditional_var_sub_constant", &m_subConstant)) {
                  m_subConstant = std::clamp(m_subConstant, 0, 9999);
                }
                ImGui::EndDisabled();
                ImGui::BeginDisabled(m_variableSubSource != 1);
                ImGui::PushID("##conditional_var_sub_variable");
                if (ImGui::EllipsesButton(m_variableSubSource != 1 ? "" : Database::instance()->variableNameAndId(m_subVariableId).c_str(), ImVec2{-1, 0})) {
                  m_pickerType = 2;
                  m_variablePicker.emplace(trNOOP("Variables"), Database::instance()->system.variables(), m_subVariableId);
                  m_variablePicker->setOpen(true);
                }
                ImGui::PopID();
                ImGui::EndDisabled();
              }
              ImGui::EndVertical();
            }
            ImGui::EndHorizontal();
          }
          ImGui::EndDisabled();
        }
        ImGui::EndVertical();
        ImGui::BeginHorizontal("##conditional_branch_data_tab_right_self_switch", {-1, 0}, 0);
        {
          // Self Switches
          ImGui::BeginDisabled(m_conditionType != 2);
          ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * .33f);
          if (ImGui::BeginCombo("##conditional_branch_data_tab_self_switch_list", m_conditionType != 2 ? "" : m_selfSw.c_str())) {
            for (const auto self : {"A", "B", "C", "D"}) {
              if (ImGui::Selectable(self, self == m_selfSw)) {
                m_selfSw = self;
              }
            }
            ImGui::EndCombo();
          }
          const auto label = trNOOP("is");
          ImGui::TextUnformatted(label);
          ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * .5f - (ImGui::CalcTextSize(label).x - ImGui::GetStyle().FramePadding.x));
          if (ImGui::BeginCombo("##conditional_branch_data_tab_self_switch_sub_selection", DecodeEnumName(magic_enum::enum_value<ValueControl>(m_selfSwValue)).c_str())) {
            for (auto& dir : magic_enum::enum_values<ValueControl>()) {
              if (const bool isSelected = m_selfSwValue == static_cast<int>(dir); ImGui::Selectable(DecodeEnumName(dir).c_str(), isSelected)) {
                m_selfSwValue = static_cast<int>(dir);
                if (isSelected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::EndDisabled();
        }
        ImGui::EndHorizontal();
        ImGui::BeginChild("##conditional_branch_data_tab_right_timer", {-1, 0},
                          ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_NoBackground);
        {
          ImGui::BeginDisabled(m_conditionType != 3);
          {
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .33f);
            if (ImGui::BeginCombo("##conditional_branch_timer_comparison", DecodeEnumName(magic_enum::enum_value<TimerComparisonType>(m_timerOperation)).c_str())) {
              for (auto& dir : magic_enum::enum_values<TimerComparisonType>()) {
                if (bool isSelected = m_selfSwValue == magic_enum::enum_index(dir).value(); ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), isSelected)) {
                  m_timerOperation = magic_enum::enum_index(dir).value();
                  if (isSelected)
                    ImGui::SetItemDefaultFocus();
                }
              }
              ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .5f);
            // TL-NOTE: Minutes
            if (ImGui::SpinInt("##conditional_branch_timer_min", &m_timerMin, 1, 5, trNOOP("%d min"))) {
              m_timerMin = std::clamp(m_timerMin, 0, 99);
            }
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-1);
            // TL-NOTE: Seconds
            if (ImGui::SpinInt("##conditional_branch_timer_sec", &m_timerSec, 1, 5, trNOOP("%d sec"))) {
              m_timerSec = std::clamp(m_timerSec, 0, 59);
            }
          }
          ImGui::EndDisabled();
        }
        ImGui::EndChild();
      }
      ImGui::EndVertical();
    }
    ImGui::EndHorizontal();
    ImGui::EndTabItem();
  }

  if (ImGui::IsItemClicked()) {
    m_selectedTab = 0;
  }
}

void Dialog_ConditionalBranch::drawActorTab() {
  if (ImGui::BeginTabItem(trNOOP("Actor"), nullptr, m_selectedTab == 1 ? ImGuiTabItemFlags_SetSelected : 0)) {
    ImGui::BeginHorizontal("##conditional_branch_actor_tab_main_layout", {-1, -1}, 0);
    {
      ImGui::BeginVertical("##conditional_branch_actor_tab_left", {-1, 0}, 0);
      {
        //
        ImGui::RadioButton(trNOOP("Actor"), &m_conditionType, 4);
      }
      ImGui::EndVertical();
      ImGui::BeginVertical("##conditional_branch_actor_tab_right", {-1, 0}, 0);
      {
        ImGui::BeginDisabled(m_conditionType != 4);
        {
          ImGui::PushID("##conditional_actor_condition");
          if (ImGui::EllipsesButton(m_conditionType != 4 ? "" : Database::instance()->actorNameAndId(m_actorSelection).c_str(), {-1, 0})) {

            m_actorPicker = ObjectPicker(trNOOP("Actor"), Database::instance()->actors.actorList(), m_actorSelection);
            m_actorPicker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::BeginHorizontal("##conditional_branch_actor_tab_right_inner", {-1, 0}, 0);
          {
            ImGui::BeginVertical("##conditional_branch_actor_tab_right_inner_left", {-1, 0}, 0);
            {
              ImGui::RadioButton(trNOOP("In the party"), &m_actorSubSelection, 0);
              ImGui::RadioButton(trNOOP("Name"), &m_actorSubSelection, 1);
              ImGui::RadioButton(trNOOP("Class"), &m_actorSubSelection, 2);
              ImGui::RadioButton(trNOOP("Skill"), &m_actorSubSelection, 3);
              ImGui::RadioButton(trNOOP("Weapon"), &m_actorSubSelection, 4);
              ImGui::RadioButton(trNOOP("Armor"), &m_actorSubSelection, 5);
              ImGui::RadioButton(trNOOP("State"), &m_actorSubSelection, 6);
            }
            ImGui::EndVertical();
            ImGui::BeginVertical("##conditional_branch_actor_tab_right_inner_right", {-1, 0}, 0);
            {
              ImGui::Dummy({-1, ImGui::GetFrameHeight()});
              // Name Input
              ImGui::BeginDisabled(m_actorSubSelection != 1);
              ImGui::SetNextItemWidth(-1);
              ImGui::InputText("##conditional_branch_actor_tab_nameinput", &m_actorSelectionNameInput);
              ImGui::EndDisabled();

              // Class Button
              ImGui::BeginDisabled(m_actorSubSelection != 2);
              ImGui::PushID("##change_class_selection");
              if (ImGui::EllipsesButton(m_actorSubSelection != 2 ? "" : Database::instance()->classNameAndId(m_actorClass).c_str(), ImVec2{-1, 0})) {
                m_classPicker = ObjectPicker(trNOOP("Class"), Database::instance()->classes.classes(), m_actorClass);
                m_classPicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::EndDisabled();

              // Skill Button
              ImGui::BeginDisabled(m_actorSubSelection != 3);
              ImGui::PushID("##change_skill_selection");
              if (ImGui::EllipsesButton(m_actorSubSelection != 3 ? "" : Database::instance()->skillNameAndId(m_actorSkill).c_str(), ImVec2{-1, 0})) {
                m_skillPicker = ObjectPicker(trNOOP("Skill"), Database::instance()->skills.skills(), m_actorSkill);
                m_skillPicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::EndDisabled();

              // Weapons Button
              ImGui::BeginDisabled(m_actorSubSelection != 4);
              ImGui::PushID("##change_weapons_selection");
              if (ImGui::EllipsesButton(m_actorSubSelection != 4 ? "" : Database::instance()->weaponNameOrId(m_actorWeapon).c_str(), {-1, 0})) {
                m_weaponType = 0;
                m_weaponPicker = ObjectPicker(trNOOP("Weapon"), Database::instance()->weapons.weaponList(), m_actorWeapon);
                m_weaponPicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::EndDisabled();

              // Armors Button
              ImGui::BeginDisabled(m_actorSubSelection != 5);
              ImGui::PushID("##change_armors_selection");
              if (ImGui::EllipsesButton(m_actorSubSelection != 5 ? "" : Database::instance()->armorNameAndId(m_actorArmor).c_str(), {-1, 0})) {
                m_armorType = 0;
                m_armorPicker = ObjectPicker(trNOOP("Armor"), Database::instance()->armors.armorList(), m_actorArmor);
                m_actorPicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::EndDisabled();

              // State Button
              ImGui::BeginDisabled(m_actorSubSelection != 6);
              ImGui::PushID("##change_state_selection");
              if (ImGui::EllipsesButton(m_actorSubSelection != 6 ? "" : Database::instance()->stateNameAndId(m_actorState).c_str(), {-1, 0})) {
                m_stateType = 0;
                m_statePicker = ObjectPicker(trNOOP("States"), Database::instance()->states.states(), m_actorState);
                m_statePicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::EndDisabled();
            }
            ImGui::EndVertical();
          }
          ImGui::EndHorizontal();
        }
        ImGui::EndDisabled();
      }
      ImGui::EndVertical();
    }
    ImGui::EndHorizontal();
    ImGui::EndTabItem();
  }
  if (ImGui::IsItemClicked()) {
    m_selectedTab = 1;
  }
}

void Dialog_ConditionalBranch::drawEntityTab() {
  if (ImGui::BeginTabItem(trNOOP("Entity"), nullptr, m_selectedTab == 2 ? ImGuiTabItemFlags_SetSelected : 0)) {
    ImGui::BeginHorizontal("##conditional_branch_entity_tab_main_layout", {-1, -1}, 0);
    {
      ImGui::BeginVertical("##conditional_branch_entity_tab_main_left_layout", {-1, 0}, 0);
      {
        ImGui::RadioButton(trNOOP("Enemy"), &m_conditionType, 5);
        ImGui::Dummy({-1, ImGui::GetFrameHeight()});
        ImGui::Dummy({-1, ImGui::GetFrameHeight()});
        ImGui::RadioButton(trNOOP("Character"), &m_conditionType, 6);
        ImGui::RadioButton(trNOOP("Vehicle"), &m_conditionType, 13);
      }
      ImGui::EndVertical();
      ImGui::BeginVertical("##conditional_branch_entity_tab_main_right_layout", {-1, 0}, 0);
      {
        ImGui::BeginVertical("##conditional_branch_entity_tab_main_right_enemy_layout", {-1, 0}, 0);
        {
          ImGui::BeginDisabled(m_conditionType != 5);
          {
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##conditional_branch_entity_tab_main_right_enemy_enemy_list",
                                  (m_conditionType != 5 ? "" : "#" + std::to_string(m_enemySelection + 1) + " " + Database::instance()->troopMemberName(m_troopId, m_enemySelection)).c_str())) {
              for (int i = 0; i < 9; ++i) {
                if (ImGui::Selectable(std::format("#{} {}", i + 1, Database::instance()->troopMemberName(m_troopId, i)).c_str(), i == m_enemySelection)) {
                  m_enemySelection = i;
                }
              }
              ImGui::EndCombo();
            }
            ImGui::BeginHorizontal("##conditional_branch_entity_tab_main_right_enemy_layout_inner", {-1, 0}, 0);
            {
              ImGui::BeginVertical("##conditional_branch_entity_tab_main_right_enemy_radios", {-1, 0}, 0);
              {
                ImGui::RadioButton("Appeared", &m_enemySubSelection, 0);
                ImGui::RadioButton("State", &m_enemySubSelection, 1);
              }
              ImGui::EndVertical();
              ImGui::BeginVertical("##conditional_branch_entity_tab_main_right_enemy_values", {-1, 0}, 0);
              {
                ImGui::Dummy({-1, ImGui::GetFrameHeight()});
                ImGui::BeginDisabled(m_enemySubSelection != 1);
                ImGui::PushID("##change_state_selection");
                if (ImGui::EllipsesButton(m_enemySubSelection != 1 ? "" : Database::instance()->stateNameAndId(m_actorState).c_str(), {-1, 0})) {
                  m_stateType = 1;
                  m_statePicker = ObjectPicker(trNOOP("States"), Database::instance()->states.states(), m_actorState);
                  m_statePicker->setOpen(true);
                }
                ImGui::PopID();
                ImGui::EndDisabled();
              }
              ImGui::EndVertical();
            }
            ImGui::EndHorizontal();
          }
          ImGui::EndDisabled();
        }
        ImGui::EndVertical();
        ImGui::BeginHorizontal("##conditional_branch_entity_tab_main_right_character_layout", {-1, 0}, 0);
        {
          ImGui::BeginDisabled(m_conditionType != 6 || !Database::instance()->mapInfos.currentMap() || !Database::instance()->mapInfos.currentMap()->map());
          {
            const auto label = trNOOP("is facing");
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .5f);
            if (ImGui::BeginCombo("##conditional_branch_entity_tab_main_right_character_list", m_conditionType != 6 ? "" : Database::instance()->eventNameOrId(m_characterSelection).c_str())) {
              const auto map = Database::instance()->mapInfos.currentMap()->map();
              for (int i = -1; i < static_cast<int>(map->events().size()); ++i) {
                if (i > 0 && !map->event(i)) {
                  continue;
                }
                const bool selected = m_characterSelection == i;
                if (ImGui::Selectable(Database::instance()->eventNameOrId(i).c_str(), selected)) {
                  m_characterSelection = i;
                }
                if (selected) {
                  ImGui::SetItemDefaultFocus();
                }
              }
              ImGui::EndCombo();
            }
            ImGui::TextUnformatted(label);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x));
            if (ImGui::BeginCombo("##conditional_branch_entity_tab_main_right_direction_selection",
                                  m_conditionType != 6 ? "" : DecodeEnumName(magic_enum::enum_value<Direction>(m_characterDirection)).c_str())) {
              for (auto& dir : magic_enum::enum_values<Direction>()) {
                if (dir == Direction::Retain) {
                  continue;
                }

                if (bool is_selected = m_characterDirection == static_cast<int>(dir); ImGui::Selectable(DecodeEnumName(dir).c_str(), is_selected)) {
                  m_characterDirection = magic_enum::enum_index(dir).value();
                  if (is_selected)
                    ImGui::SetItemDefaultFocus();
                }
              }
              ImGui::EndCombo();
            }
          }
          ImGui::EndDisabled();
        }
        ImGui::EndHorizontal();
        ImGui::BeginHorizontal("##conditional_branch_entity_tab_main_right_vehicle_layout", {-1, 0}, 0);
        {
          ImGui::BeginDisabled(m_conditionType != 13);
          {
            const auto label = trNOOP("is driven");
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .5f);
            if (ImGui::BeginCombo("##conditional_branch_entity_tab_main_right_vehicle_location_selection",
                                  m_conditionType != 13 ? "" : DecodeEnumName(magic_enum::enum_value<VehicleType>(m_vehicleSelection)).c_str())) {
              for (auto& vehicle : magic_enum::enum_values<VehicleType>()) {
                const bool isSelected = m_vehicleSelection == magic_enum::enum_index(vehicle).value();
                if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(vehicle)).c_str(), isSelected)) {
                  m_vehicleSelection = magic_enum::enum_index(vehicle).value();
                }
                if (isSelected) {
                  ImGui::SetItemDefaultFocus();
                }
              }
              ImGui::EndCombo();
            }
            ImGui::TextUnformatted(label);
          }
          ImGui::EndDisabled();
        }
        ImGui::EndHorizontal();
      }
      ImGui::EndVertical();
    }
    ImGui::EndHorizontal();
    ImGui::EndTabItem();
  }
  if (ImGui::IsItemClicked()) {
    m_selectedTab = 2;
  }
}

void Dialog_ConditionalBranch::drawMiscTab() {
  if (ImGui::BeginTabItem(trNOOP("Misc"), nullptr, m_selectedTab == 3 ? ImGuiTabItemFlags_SetSelected : 0)) {
    ImGui::BeginVertical("##conditional_branch_misc_tab_main_layout", {-1, -1}, 0);
    {
      ImGui::BeginHorizontal("##conditional_branch_misc_tab_main_layout_inner", {-1, -1}, 0);
      {
        ImGui::BeginVertical("##conditional_branch_misc_tab_main_left_layout", {-1, 0}, 0);
        {
          ImGui::RadioButton(trNOOP("Gold"), &m_conditionType, 7);
          ImGui::RadioButton(trNOOP("Item"), &m_conditionType, 8);
          ImGui::RadioButton(trNOOP("Weapon"), &m_conditionType, 9);
          ImGui::Dummy(ImVec2{0, ImGui::GetFrameHeight()});
          ImGui::RadioButton(trNOOP("Armor"), &m_conditionType, 10);
          ImGui::Dummy(ImVec2{0, ImGui::GetFrameHeight()});
          ImGui::RadioButton(trNOOP("Button"), &m_conditionType, 11);
          ImGui::RadioButton(trNOOP("Script:"), &m_conditionType, 12);
        }
        ImGui::EndVertical();
        ImGui::BeginVertical("##conditional_branch_misc_tab_main_right_layout", {-1, 0}, 0);
        {
          ImGui::BeginHorizontal("##conditional_branch_misc_tab_main_left_gold", {-1, 0}, 0);
          {
            ImGui::BeginDisabled(m_conditionType != 7);
            {
              ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .33f);
              if (ImGui::BeginCombo("##conditional_gold_operation", m_conditionType != 7 ? "" : DecodeEnumName(magic_enum::enum_value<GoldComparisonType>(m_goldOperation)).c_str())) {
                for (auto& operation : magic_enum::enum_values<GoldComparisonType>()) {
                  bool isSelected = m_goldOperation == magic_enum::enum_index(operation).value();
                  if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(operation)).c_str(), isSelected)) {
                    m_goldOperation = magic_enum::enum_index(operation).value();
                  }
                  if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                  }
                }
                ImGui::EndCombo();
              }
              ImGui::BeginChild("##conditional_gold_value", {-1, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_NavFlattened,
                                ImGuiWindowFlags_NoBackground);
              {
                ImGui::SetNextItemWidth(-1);
                if (ImGui::SpinInt("##conditional_gold_selection", &m_goldSelection, 1, 100, std::format("%d {}", Database::instance()->instance()->system.currencyUnit()).c_str())) {
                  m_goldSelection = std::clamp(m_goldSelection, 0, 9999999);
                }
              }
              ImGui::EndChild();
            }
            ImGui::EndDisabled();
          }
          ImGui::EndHorizontal();
          ImGui::BeginVertical("##conditional_branch_misc_tab_main_left_item", {-1, 0}, 0);
          {
            ImGui::BeginDisabled(m_conditionType != 8);
            {
              ImGui::PushID("##conditional_item_selection");
              if (ImGui::EllipsesButton(m_conditionType != 8 ? "" : Database::instance()->itemNameOrId(m_itemSelection).c_str(), {-1, 0})) {
                m_itemPicker = ObjectPicker<Item>("Items"sv, Database::instance()->items.items(), m_itemSelection);
                m_itemPicker->setOpen(true);
              }
              ImGui::PopID();
            }
            ImGui::EndDisabled();
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##conditional_branch_misc_tab_main_left_weapon", {-1, 0}, 0);
          {
            // Weapon
            ImGui::BeginDisabled(m_conditionType != 9);
            {
              ImGui::PushID("##conditional_weapon_selection");
              if (ImGui::EllipsesButton(m_conditionType != 9 ? "" : Database::instance()->weaponNameOrId(m_weaponSelection).c_str(), {-1, 0})) {
                m_weaponType = 1;
                m_weaponPicker = ObjectPicker(trNOOP("Weapons"), Database::instance()->weapons.weapons(), m_weaponSelection);
                m_weaponPicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::Checkbox(std::format("{}##conditional_branch_misc_tab_weapon_checkbox", trNOOP("Include Equipment")).c_str(), &m_weaponInclude);
            }
            ImGui::EndDisabled();
          }
          ImGui::EndVertical();
          ImGui::BeginVertical("##conditional_branch_misc_tab_main_left_armor", {-1, 0}, 0);
          {
            // Armor
            ImGui::BeginDisabled(m_conditionType != 10);
            {
              ImGui::PushID("##conditional_armor_selection");
              if (ImGui::EllipsesButton(m_conditionType != 10 ? "" : Database::instance()->armorNameOrId(m_armorSelection).c_str(), {-1, 0})) {
                m_armorType = 1;
                m_armorPicker = ObjectPicker(trNOOP("Armors"), Database::instance()->armors.armors(), m_armorSelection);
                m_armorPicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::Checkbox(std::format("{}##conditional_branch_misc_tab_armor_checkbox", trNOOP("Include Equipment")).c_str(), &m_armorInclude);
            }
            ImGui::EndDisabled();
          }
          ImGui::EndVertical();
          ImGui::BeginHorizontal("##conditional_branch_misc_tab_main_left_button", {-1, 0}, 0);
          {
            // Button
            ImGui::BeginDisabled(m_conditionType != 11);
            {
              ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * .33f);
              if (ImGui::BeginCombo("##conditional_branch_misc_tab_main_left_button_selection",
                                    m_conditionType != 11 ? "" : DecodeEnumName(magic_enum::enum_value<Button>(m_buttonSelection)).c_str())) {
                for (auto& button : magic_enum::enum_values<Button>()) {
                  const bool isSelected = m_buttonSelection == static_cast<int>(button);
                  if (ImGui::Selectable(DecodeEnumName(button).c_str(), isSelected)) {
                    m_buttonSelection = static_cast<int>(button);
                  }
                  if (isSelected)
                    ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
              }
              ImGui::TextUnformatted(trNOOP("is pressed down"));
            }
            ImGui::EndDisabled();
          }
          ImGui::EndHorizontal();
        }
        ImGui::EndVertical();
      }
      ImGui::EndHorizontal();
      ImGui::BeginDisabled(m_conditionType != 12);
      {
        if (ImGui::BeginChild("##source_text_child", {-1, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()},
                              ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove)) {
          m_script.SetImGuiChildIgnored(true);
          m_script.SetHandleKeyboardInputs(m_conditionType == 12);
          m_script.SetHandleMouseInputs(m_conditionType == 12);
          m_script.Render("##source_text", {-1, -1});
        }
        ImGui::EndChild();
      }
      ImGui::EndDisabled();
    }
    ImGui::EndVertical();
    ImGui::EndTabItem();
  }
  if (ImGui::IsItemClicked()) {
    m_selectedTab = 3;
  }
}

void Dialog_ConditionalBranch::drawPickers() {
  if (m_variablePicker) {
    if (const auto [closed, confirmed] = m_variablePicker->draw(); closed) {
      if (confirmed) {
        if (m_pickerType == 0) {
          m_switchId = m_variablePicker->selection();
        } else if (m_pickerType == 2) {
          m_subVariableId = m_variablePicker->selection();
        } else {
          m_variableId = m_variablePicker->selection();
        }
      }
      m_variablePicker.reset();
    }
  }

  if (m_actorPicker) {
    if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
      if (confirmed) {
        m_actorSelection = m_actorPicker->selection();
      }
      m_actorPicker.reset();
    }
  }
  if (m_classPicker) {
    if (const auto [closed, confirmed] = m_classPicker->draw(); closed) {
      if (confirmed) {
        m_actorClass = m_classPicker->selection();
      }
      m_classPicker.reset();
    }
  }
  if (m_skillPicker) {
    if (const auto [closed, confirmed] = m_skillPicker->draw(); closed) {
      if (confirmed) {
        m_actorSkill = m_skillPicker->selection();
      }
      m_skillPicker.reset();
    }
  }
  if (m_weaponPicker) {
    if (const auto [closed, confirmed] = m_weaponPicker->draw(); closed) {
      if (confirmed) {
        if (m_weaponType == 0) {
          m_actorWeapon = m_weaponPicker->selection();
        } else {
          m_weaponSelection = m_weaponPicker->selection();
        }
      }
      m_weaponPicker.reset();
    }
  }
  if (m_armorPicker) {
    if (const auto [closed, confirmed] = m_armorPicker->draw(); closed) {
      if (confirmed) {
        if (m_armorType == 0) {
          m_actorArmor = m_armorPicker->selection();
        } else {
          m_armorSelection = m_armorPicker->selection();
        }
      }
      m_armorPicker.reset();
    }
  }
  if (m_statePicker) {
    if (const auto [closed, confirmed] = m_statePicker->draw(); closed) {
      if (confirmed) {
        if (m_stateType == 0) {
          m_actorState = m_statePicker->selection();
        } else {
          m_enemySubState = m_statePicker->selection();
        }
      }
      m_statePicker.reset();
    }
  }

  if (m_itemPicker) {
    if (const auto [closed, confirmed] = m_itemPicker->draw(); closed) {
      if (confirmed) {
        m_itemSelection = m_itemPicker->selection();
      }
      m_itemPicker.reset();
    }
  }
}

void Dialog_ConditionalBranch::drawDialogButtons() {
  ImGui::Spring();
  ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
  ImGui::BeginHorizontal("##conditional_branch_dialog_buttons_layout");
  {
    ImGui::Checkbox(trNOOP("Create Else Branch"), &m_elseBranch);
    ImGui::Spring();
    if (const auto ret = ImGui::ButtonGroup("##conditional_branch_dialog_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
      m_confirmed = true;
      m_command->type = static_cast<ConditionType>(m_conditionType);
      if (m_command->type == ConditionType::Switch) {
        m_command->globalSwitch.switchIdx = m_switchId;
        m_command->globalSwitch.checkIfOn = static_cast<ValueControl>(m_switchValue);
      } else if (m_command->type == ConditionType::Variable) {
        m_command->variable.id = m_variableId;
        m_command->variable.source = static_cast<VariableComparisonSource>(m_variableSubSource);
        m_command->variable.comparison = static_cast<VariableComparisonType>(m_variableValue);
        if (m_command->variable.source == VariableComparisonSource::Constant) {
          m_command->variable.constant = m_subConstant;
        } else {
          m_command->variable.otherId = m_subVariableId;
        }
      } else if (m_command->type == ConditionType::Self_Switch) {
        m_command->selfSw = m_selfSw;
        m_command->selfSwitch.checkIfOn = static_cast<ValueControl>(m_selfSwValue);
      } else if (m_command->type == ConditionType::Timer) {
        m_command->timer.sec = m_timerSec + (m_timerMin * 60);
        m_command->timer.comparison = static_cast<TimerComparisonType>(m_timerOperation);
      } else if (m_command->type == ConditionType::Actor) {
        m_command->actor.id = m_actorSelection;
        m_command->actor.type = static_cast<ActorConditionType>(m_actorSubSelection);
        if (m_command->actor.type == ActorConditionType::In_The_Party) {
          m_command->actor.checkId = 0;
        }
        if (m_command->actor.type == ActorConditionType::Name) {
          m_command->name = m_actorSelectionNameInput;
        }
        if (m_command->actor.type == ActorConditionType::Class) {
          m_command->actor.checkId = m_actorClass;
        }
        if (m_command->actor.type == ActorConditionType::Skill) {
          m_command->actor.checkId = m_actorSkill;
        }
        if (m_command->actor.type == ActorConditionType::Weapon) {
          m_command->actor.checkId = m_actorWeapon;
        }
        if (m_command->actor.type == ActorConditionType::Armor) {
          m_command->actor.checkId = m_actorArmor;
        }
        if (m_command->actor.type == ActorConditionType::State) {
          m_command->actor.checkId = m_actorState;
        }
      } else if (m_command->type == ConditionType::Enemy) {
        m_command->enemy.id = m_enemySelection;
        m_command->enemy.type = static_cast<EnemyConditionType>(m_enemySubSelection);
        if (m_command->enemy.type == EnemyConditionType::State) {
          m_command->enemy.stateId = m_enemySubState;
        } else {
          m_command->enemy.stateId = 0;
        }
      } else if (m_command->type == ConditionType::Character) {
        m_command->character.id = m_characterSelection;
        m_command->character.facing = static_cast<Direction>((m_characterDirection * 2));
      } else if (m_command->type == ConditionType::Vehicle) {
        m_command->vehicle.id = static_cast<VehicleType>(m_vehicleSelection);
      } else if (m_command->type == ConditionType::Gold) {
        m_command->gold.type = static_cast<GoldComparisonType>(m_goldOperation);
        m_command->gold.value = m_goldSelection;
      } else if (m_command->type == ConditionType::Item) {
        m_command->item.id = m_itemSelection;
      } else if (m_command->type == ConditionType::Weapon) {
        m_command->equip.equipId = m_weaponSelection;
        m_command->equip.includeEquipment = m_weaponInclude;
      } else if (m_command->type == ConditionType::Armor) {
        m_command->equip.equipId = m_armorSelection;
        m_command->equip.includeEquipment = m_armorInclude;
      } else if (m_command->type == ConditionType::Button) {
        m_command->button = static_cast<Button>(m_buttonSelection);
      } else if (m_command->type == ConditionType::Script) {
        m_command->script = m_script.GetText();
      }
      ImGui::CloseCurrentPopup();
      setOpen(false);
    } else if (ret == 1) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
  }
  ImGui::EndHorizontal();
}