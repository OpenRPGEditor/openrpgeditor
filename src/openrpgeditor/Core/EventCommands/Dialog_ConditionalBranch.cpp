
#include "Core/EventCommands/Dialog_ConditionalBranch.hpp"

#include "Core/Log.hpp"
#include "Database/Database.hpp"
#include "imgui.h"

std::tuple<bool, bool> Dialog_ConditionalBranch::draw() {

  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{800, 600});
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (closed) {
        if (confirmed) {
          if (m_picker_type == 0) {
            m_switch_id = picker->selection();
          } else if (m_picker_type == 2) {
            m_sub_variable_id = picker->selection();
          } else {
            m_variable_id = picker->selection();
          }
        }
        picker.reset();
      }
    }

    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (closed) {
        if (confirmed) {
          m_actor_sub_selection = actor_picker->selection();
        }
        actor_picker.reset();
      }
    }
    if (class_picker) {
      auto [closed, confirmed] = class_picker->draw();
      if (closed) {
        if (confirmed) {
          m_actor_class = class_picker->selection();
        }
        class_picker.reset();
      }
    }
    if (skill_picker) {
      auto [closed, confirmed] = skill_picker->draw();
      if (closed) {
        if (confirmed) {
          m_actor_skill = skill_picker->selection();
        }
        skill_picker.reset();
      }
    }
    if (weapon_picker) {
      auto [closed, confirmed] = weapon_picker->draw();
      if (closed) {
        if (confirmed) {
          if (m_weapon_type == 0) {
            m_actor_weapon = weapon_picker->selection();
          } else {
            m_weapon_selection = weapon_picker->selection();
          }
        }
        weapon_picker.reset();
      }
    }
    if (armor_picker) {
      auto [closed, confirmed] = armor_picker->draw();
      if (closed) {
        if (confirmed) {
          if (m_armor_type == 0) {
            m_actor_armor = armor_picker->selection();
          } else {
            m_armor_selection = armor_picker->selection();
          }
        }
        armor_picker.reset();
      }
    }
    if (state_picker) {
      auto [closed, confirmed] = state_picker->draw();
      if (closed) {
        if (confirmed) {
          if (m_state_type == 0) {
            m_actor_state = state_picker->selection();
          } else {
            m_enemy_sub_state = state_picker->selection();
          }
        }
        state_picker.reset();
      }
    }

    if (item_picker) {
      auto [closed, confirmed] = item_picker->draw();
      if (closed) {
        if (confirmed) {
          m_item_selection = item_picker->selection();
        }
        item_picker.reset();
      }
    }
    if (ImGui::BeginTabBar("##conditional_branch_dialog")) {

      // ImVec2 size = ImVec2{((ImGui::GetContentRegionAvail().x / 2) / 2) + 50, 0};

      if (ImGui::BeginTabItem("Data")) {
        ImGui::BeginGroup();
        {
          ImGui::RadioButton("Switch", &m_conditionType, 0);
          ImGui::RadioButton("Variable", &m_conditionType, 1);
          ImGui::Dummy(ImVec2{0, 50});
          ImGui::RadioButton("Self Switch", &m_conditionType, 2);
          ImGui::RadioButton("Timer", &m_conditionType, 3);
          ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        {

          // Switches
          std::string text = m_conditionType != 0 ? "##conditional_switch_empty" : Database::instance()->switchNameOrId(m_switch_id).c_str();
          ImGui::PushID("##conditional_switch_id");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x + 50) - 16);
          ImGui::BeginDisabled(m_conditionType != 0);
          if (ImGui::Button(text.c_str(), ImVec2{200, 0})) {
            m_picker_type = 0;
            picker.emplace("Switches", Database::instance()->system.switches, m_switch_id);
            picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::SameLine();
          ImGui::Text("is");
          ImGui::SameLine();
          ImGui::PushItemWidth((100));
          if (ImGui::BeginCombo("##conditional_switch_sub_selection", DecodeEnumName(magic_enum::enum_value<ValueControl>(m_switch_value)).c_str())) {
            for (auto& dir : magic_enum::enum_values<ValueControl>()) {
              bool is_selected = m_switch_value == magic_enum::enum_index(dir).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                m_switch_value = magic_enum::enum_index(dir).value();
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::EndDisabled();

          // Variables
          ImGui::BeginDisabled(m_conditionType != 1);
          text = m_conditionType != 1 ? "##conditional_variable_id" : Database::instance()->variableNameAndId(m_variable_id).c_str();
          ImGui::PushID("##controlvariable_id2");
          if (ImGui::Button(text.c_str(), ImVec2{200, 0})) {
            m_picker_type = 1;
            picker.emplace("Variables", Database::instance()->system.variables, m_variable_id);
            picker->setOpen(true);
          }
          ImGui::PopID();

          ImGui::SameLine();
          ImGui::PushItemWidth((100));
          if (ImGui::BeginCombo("##conditional_variable_sub_selection", DecodeEnumName(magic_enum::enum_value<VariableComparisonType>(m_variable_value)).c_str())) {
            for (auto& dir : magic_enum::enum_values<VariableComparisonType>()) {
              bool is_selected = m_variable_value == magic_enum::enum_index(dir).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                m_variable_value = magic_enum::enum_index<VariableComparisonType>(dir).value();
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::EndDisabled();

          ImGui::BeginDisabled(m_conditionType != 1);
          ImGui::BeginGroup();
          {
            ImGui::RadioButton("Constant", &m_variable_subSource, 0);
            ImGui::RadioButton("Variable##2", &m_variable_subSource, 1);
            ImGui::EndGroup();
          }
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::BeginDisabled(m_variable_subSource != 0);
            ImGui::SetNextItemWidth(150);
            if (ImGui::InputInt("##conditional_var_sub_constant", &m_sub_constant)) {
              if (m_sub_constant > 9999)
                m_sub_constant = 9999;
              if (m_sub_constant < 0)
                m_sub_constant = 0;
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_variable_subSource != 1);
            ImGui::PushID("##conditional_var_sub_variable");
            if (ImGui::Button(m_variable_subSource != 1 ? "" : Database::instance()->variableNameAndId(m_sub_variable_id).c_str(),

                              ImVec2{(200), 0})) {
              m_picker_type = 2;
              picker.emplace("Variables", Database::instance()->system.variables, m_sub_variable_id);
              picker->setOpen(true);
            }
            ImGui::PopID();
            ImGui::EndDisabled();
            ImGui::EndGroup();
          }
          ImGui::EndDisabled();
          // Self Switches
          ImGui::BeginDisabled(m_conditionType != 2);
          ImGui::SetNextItemWidth(120);
          if (ImGui::BeginCombo("##conditional_selfswitch_list", m_conditionType != 2 ? "" : m_selfSw.c_str())) {
            for (const auto self : {"A", "B", "C", "D"}) {
              if (ImGui::Selectable(self, self == m_selfSw)) {
                m_selfSw = self;
              }
            }
            ImGui::EndCombo();
          }
          ImGui::SameLine();
          ImGui::Text("is");
          ImGui::SameLine();
          ImGui::PushItemWidth((100));
          if (ImGui::BeginCombo("##conditional_selfswitch_sub_selection", DecodeEnumName(magic_enum::enum_value<ValueControl>(m_selfSw_value)).c_str())) {
            for (auto& dir : magic_enum::enum_values<ValueControl>()) {
              bool is_selected = m_selfSw_value == magic_enum::enum_index(dir).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                m_selfSw_value = magic_enum::enum_index(dir).value();
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::EndDisabled();

          ImGui::BeginDisabled(m_conditionType != 3);
          ImGui::SetNextItemWidth(120);
          if (ImGui::BeginCombo("##conditional_selfswitch_sub_comparison", DecodeEnumName(magic_enum::enum_value<TimerComparisonType>(m_timer_operation)).c_str())) {
            for (auto& dir : magic_enum::enum_values<TimerComparisonType>()) {
              bool is_selected = m_selfSw_value == magic_enum::enum_index(dir).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                m_timer_operation = magic_enum::enum_index(dir).value();
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::SetNextItemWidth(100);
            if (ImGui::InputInt("min", &m_timer_min)) {
              if (m_timer_min > 99) {
                m_timer_min = 99;
              }
              if (m_timer_min < 0) {
                m_timer_min = 0;
              }
            }
            ImGui::SetNextItemWidth(100);
            if (ImGui::InputInt("sec", &m_timer_sec)) {
              if (m_timer_min > 59) {
                m_timer_min = 59;
              }
              if (m_timer_min < 0) {
                m_timer_min = 0;
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndDisabled();

          ImGui::EndGroup();
        }
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Actor")) {
        ImGui::BeginGroup();
        {
          ImGui::RadioButton("Actor", &m_conditionType, 4);
          ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::BeginGroup();
        {
          ImGui::PushID("##conditional_actor_condition");
          if (ImGui::Button(m_conditionType != 4 ? "" : Database::instance()->actorNameAndId(m_actor_selection).c_str(), {(160), 0})) {

            actor_picker = ObjectPicker<Actor>("Actor"sv, Database::instance()->actors.actorList(), m_actor_selection);
            actor_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::BeginDisabled(m_conditionType != 4);
          ImGui::RadioButton("In the party", &m_actor_sub_selection, 0);
          ImGui::EndDisabled();
          ImGui::EndGroup();
        }
        ImGui::SetCursorPosX(cursorPos.x);
        ImGui::BeginGroup();
        {
          ImGui::BeginDisabled(m_conditionType != 4);
          ImGui::RadioButton("Name", &m_actor_sub_selection, 1);
          ImGui::RadioButton("Class", &m_actor_sub_selection, 2);
          ImGui::RadioButton("Skill", &m_actor_sub_selection, 3);
          ImGui::RadioButton("Weapon", &m_actor_sub_selection, 4);
          ImGui::RadioButton("Armor", &m_actor_sub_selection, 5);
          ImGui::RadioButton("State", &m_actor_sub_selection, 6);
          ImGui::EndDisabled();
          ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImGui::BeginDisabled(m_actor_sub_selection != 1);
          ImGui::SetNextItemWidth(180);
          ImGui::InputText("##conditional_actor_nameinput", &m_actor_selection_nameinput);
          ImGui::EndDisabled();

          // Class Button
          ImGui::BeginDisabled(m_actor_sub_selection != 2);
          ImGui::PushID("##change_class_selection");
          if (ImGui::Button(m_actor_sub_selection != 2 ? "" : Database::instance()->className(m_actor_class).c_str(), ImVec2{180, 0})) {
            class_picker = ObjectPicker<Class>("Class"sv, Database::instance()->classes.classes(), m_actor_class);
            class_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          // Skill Button
          ImGui::BeginDisabled(m_actor_sub_selection != 3);
          ImGui::PushID("##change_skill_selection");
          if (ImGui::Button(m_actor_sub_selection != 3 ? "" : (std::format("{:04} ", m_actor_skill) + Database::instance()->skillNameOrId(m_actor_skill)).c_str(), ImVec2{180, 0})) {
            skill_picker = ObjectPicker<Skill>("Skill"sv, Database::instance()->skills.skills(), m_actor_skill);
            skill_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          // Weapons Button
          ImGui::BeginDisabled(m_actor_sub_selection != 4);
          ImGui::PushID("##change_weapons_selection");
          if (ImGui::Button(m_actor_sub_selection != 4 ? "" : Database::instance()->weaponNameOrId(m_actor_weapon).c_str(), {180, 0})) {
            m_weapon_type = 0;
            weapon_picker = ObjectPicker<Weapon>("Weapon"sv, Database::instance()->weapons.weaponList(), m_actor_weapon);
            weapon_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          // Armors Button
          ImGui::BeginDisabled(m_actor_sub_selection != 5);
          ImGui::PushID("##change_armors_selection");
          if (ImGui::Button(m_actor_sub_selection != 5 ? "" : Database::instance()->armorNameOrId(m_actor_armor).c_str(), {180, 0})) {
            m_armor_type = 0;
            armor_picker = ObjectPicker<Armor>("Armor"sv, Database::instance()->armors.armorList(), m_actor_armor);
            actor_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          // State Button
          ImGui::BeginDisabled(m_actor_sub_selection != 6);
          ImGui::PushID("##change_state_selection");
          if (ImGui::Button(m_actor_sub_selection != 6 ? "" : Database::instance()->stateNameOrId(m_actor_state).c_str(), {180, 0})) {
            m_state_type = 0;
            state_picker = ObjectPicker<State>("States"sv, Database::instance()->states.states(), m_actor_state);
            state_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          ImGui::EndGroup();
        }
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Entity")) {
        ImGui::BeginGroup();
        {
          ImGui::RadioButton("Enemy", &m_conditionType, 5);
          ImGui::Dummy(ImVec2{0, 60});
          ImGui::RadioButton("Character", &m_conditionType, 6);
          ImGui::Dummy(ImVec2{0, 40});
          ImGui::RadioButton("Vehicle", &m_conditionType, 13);
          ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImGui::BeginDisabled(m_conditionType != 5);

          ImGui::SetNextItemWidth(180);
          if (ImGui::BeginCombo("##gamedata_enemy_list",
                                (m_conditionType != 5 ? "" : "#" + std::to_string(m_enemy_selection) + " " + Database::instance()->troopMemberName(0, m_enemy_selection)).c_str())) {
            for (int i = 1; i < 9; ++i) {
              if (ImGui::Selectable(("#" + std::to_string(i) + " " + Database::instance()->troopMemberName(0, i)).c_str(), i == m_enemy_selection)) {
                m_enemy_selection = i;
              }
            }
            ImGui::EndCombo();
          }
          ImGui::RadioButton("Appeared", &m_enemy_sub_selection, 0);
          ImGui::RadioButton("State", &m_enemy_sub_selection, 1);
          ImGui::SameLine();
          ImGui::BeginDisabled(m_enemy_sub_selection != 1);
          ImGui::PushID("##change_state_selection");
          if (ImGui::Button(m_enemy_sub_selection != 1 ? "" : Database::instance()->stateNameOrId(m_actor_state).c_str(), {180, 0})) {
            m_state_type = 1;
            state_picker = ObjectPicker<State>("States"sv, Database::instance()->states.states(), m_actor_state);
            state_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          ImGui::EndDisabled();

          ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
          ImGui::BeginDisabled(m_conditionType != 6);
          ImGui::SetNextItemWidth(180);
          if (ImGui::BeginCombo("##gamedata_character", m_conditionType != 6          ? ""
                                                        : m_character_selection == -1 ? "Player"
                                                        : m_character_selection == 0
                                                            ? "This Event"
                                                            : ("EV" + std::format("{:03} ", Database::instance()->mapInfos.currentMap()->event(m_character_selection)->id())).c_str())) {

            if (ImGui::Selectable("Player", m_character_selection == -1)) {
              m_character_selection = -1;
              ImGui::SetItemDefaultFocus();
            }
            if (ImGui::Selectable("This Event", m_character_selection == 0)) {
              m_character_selection = 0;
              ImGui::SetItemDefaultFocus();
            }

            for (const auto& dataSource : Database::instance()->mapInfos.currentMap()->map()->events()) {
              if (!dataSource.has_value())
                continue;

              if (const bool is_selected = (m_character_selection == dataSource->id()); ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id())).c_str(), is_selected)) {
                m_character_selection = dataSource->id();
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::Text("is facing");
          ImGui::SameLine();
          ImGui::PushItemWidth((75));
          if (ImGui::BeginCombo("##direction_selection", m_conditionType != 6 ? "" : DecodeEnumName(magic_enum::enum_value<Direction>(m_character_direction)).c_str())) {
            for (auto& dir : magic_enum::enum_values<Direction>()) {
              if (magic_enum::enum_index(dir).value() == 0)
                continue;

              bool is_selected = m_character_direction == magic_enum::enum_index(dir).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                m_character_direction = magic_enum::enum_index(dir).value();
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::EndDisabled();

          ImGui::BeginDisabled(m_conditionType != 13);
          ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
          ImGui::SetNextItemWidth(180);
          if (ImGui::BeginCombo("##vehicle_location_selection", m_conditionType != 13 ? "" : DecodeEnumName(magic_enum::enum_value<VehicleType>(m_vehicle_selection)).c_str())) {
            for (auto& vehicle : magic_enum::enum_values<VehicleType>()) {
              bool is_selected = m_vehicle_selection == magic_enum::enum_index(vehicle).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(vehicle)).c_str(), is_selected)) {
                m_vehicle_selection = magic_enum::enum_index(vehicle).value();
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::EndDisabled();

          ImGui::EndGroup();
        }
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Misc")) {
        ImGui::BeginGroup();
        {
          ImGui::RadioButton("Gold", &m_conditionType, 7);
          ImGui::RadioButton("Item", &m_conditionType, 8);
          ImGui::RadioButton("Weapon", &m_conditionType, 9);
          ImGui::Dummy(ImVec2{0, 25});
          ImGui::RadioButton("Armor", &m_conditionType, 10);
          ImGui::Dummy(ImVec2{0, 25});
          ImGui::RadioButton("Button", &m_conditionType, 11);
          ImGui::RadioButton("Script", &m_conditionType, 12);
          ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          // Gold
          ImGui::BeginDisabled(m_conditionType != 7);
          ImGui::SetNextItemWidth(120);
          if (ImGui::BeginCombo("##conditional_gold_operation", m_conditionType != 7 ? "" : DecodeEnumName(magic_enum::enum_value<GoldComparisonType>(m_gold_operation)).c_str())) {
            for (auto& operation : magic_enum::enum_values<GoldComparisonType>()) {
              bool is_selected = m_gold_operation == magic_enum::enum_index(operation).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(operation)).c_str(), is_selected)) {
                m_gold_operation = magic_enum::enum_index(operation).value();
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(100);
          if (ImGui::InputInt("##conditional_gold_selection", &m_gold_selection)) {
            if (m_gold_selection > 9999999) {
              m_gold_selection = 9999999;
            }
            if (m_gold_selection < 0) {
              m_gold_selection = 0;
            }
          }
          ImGui::EndDisabled();

          // Item
          ImGui::BeginDisabled(m_conditionType != 8);
          ImGui::PushID("##conditional_item_selection");
          if (ImGui::Button(m_conditionType != 8 ? "" : Database::instance()->itemNameOrId(m_item_selection).c_str(), {180, 0})) {
            item_picker = ObjectPicker<Item>("Items"sv, Database::instance()->items.items(), m_item_selection);
            item_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          // Weapon
          ImGui::BeginDisabled(m_conditionType != 9);
          ImGui::PushID("##conditional_weapon_selection");
          if (ImGui::Button(m_conditionType != 9 ? "" : Database::instance()->weaponNameOrId(m_weapon_selection).c_str(), {180, 0})) {
            m_weapon_type = 1;
            weapon_picker = ObjectPicker<Weapon>("Weapons"sv, Database::instance()->weapons.weapons(), m_weapon_selection);
            weapon_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::Dummy(ImVec2{2, 0});
          ImGui::SameLine();
          ImGui::Checkbox("Include Equipment", &m_weapon_include);
          ImGui::EndDisabled();

          // Armor
          ImGui::BeginDisabled(m_conditionType != 10);
          ImGui::PushID("##conditional_armor_selection");
          if (ImGui::Button(m_conditionType != 10 ? "" : Database::instance()->armorNameOrId(m_armor_selection).c_str(), {(160), 0})) {
            m_armor_type = 1;
            armor_picker = ObjectPicker<Armor>("Armors"sv, Database::instance()->armors.armors(), m_armor_selection);
            armor_picker->setOpen(true);
          }
          ImGui::PopID();
          ImGui::Dummy(ImVec2{2, 0});
          ImGui::SameLine();
          ImGui::Checkbox("Include Equipment##2", &m_armor_include);
          ImGui::EndDisabled();

          // Button
          ImGui::BeginDisabled(m_conditionType != 11);
          ImGui::SetNextItemWidth(100);
          if (ImGui::BeginCombo("##conditional_button_selection", m_conditionType != 11 ? "" : DecodeEnumName(magic_enum::enum_value<Button>(m_button_selection)).c_str())) {
            for (auto& button : magic_enum::enum_values<Button>()) {
              bool is_selected = m_button_selection == magic_enum::enum_index(button).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(button)).c_str(), is_selected)) {
                m_button_selection = magic_enum::enum_index(button).value();
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::SameLine();
          ImGui::Text("is pressed down");
          ImGui::EndDisabled();

          // Script
          ImGui::BeginDisabled(m_conditionType != 12);
          ImGui::SetNextItemWidth(350);
          ImGui::InputText("##conditional_script", &m_script);
          ImGui::EndDisabled();
          ImGui::EndGroup();
        }
        ImGui::EndTabItem();
      }
      ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - (30 + (ImGui::GetStyle().FramePadding.y * 2) + ImGui::GetStyle().ItemSpacing.y));
      ImGui::Separator();
      ImGui::Checkbox("Create Else Branch", &m_elseBranch);
      ImGui::SetCursorPos(ImVec2{ImGui::GetContentRegionMax().x - (105 + (ImGui::GetStyle().FramePadding.x * 2) + ImGui::GetStyle().ItemSpacing.x),
                                 ImGui::GetContentRegionMax().y - (25 + (ImGui::GetStyle().FramePadding.y * 2) + ImGui::GetStyle().ItemSpacing.y)});
      ImGui::BeginGroup();
      {
        if (ImGui::Button("OK")) {
          m_confirmed = true;
          command->type = static_cast<ConditionType>(m_conditionType);
          if (command->type == ConditionType::Switch) {
            command->globalSwitch.switchIdx = m_switch_id;
            command->globalSwitch.checkIfOn = static_cast<ValueControl>(m_switch_value);
          } else if (command->type == ConditionType::Variable) {
            command->variable.id = m_variable_id;
            command->variable.source = static_cast<VariableComparisonSource>(m_variable_subSource);
            command->variable.comparison = static_cast<VariableComparisonType>(m_variable_value);
            if (command->variable.source == VariableComparisonSource::Constant) {
              command->variable.constant = m_sub_constant;
            } else {
              command->variable.otherId = m_sub_variable_id;
            }
          } else if (command->type == ConditionType::Self_Switch) {
            command->selfSw = m_selfSw;
            command->selfSwitch.checkIfOn = static_cast<ValueControl>(m_selfSw_value);
          } else if (command->type == ConditionType::Timer) {
            command->timer.sec = m_timer_sec + (m_timer_min * 60);
            command->timer.comparison = static_cast<TimerComparisonType>(m_timer_operation);
          } else if (command->type == ConditionType::Actor) {
            command->actor.id = m_actor_selection;
            command->actor.type = static_cast<ActorConditionType>(m_actor_sub_selection);
            if (command->actor.type == ActorConditionType::In_The_Party) {
              command->actor.checkId = 0;
            }
            if (command->actor.type == ActorConditionType::Name) {
              command->name = m_actor_selection_nameinput;
            }
            if (command->actor.type == ActorConditionType::Class) {
              command->actor.checkId = m_actor_class;
            }
            if (command->actor.type == ActorConditionType::Skill) {
              command->actor.checkId = m_actor_skill;
            }
            if (command->actor.type == ActorConditionType::Weapon) {
              command->actor.checkId = m_actor_weapon;
            }
            if (command->actor.type == ActorConditionType::Armor) {
              command->actor.checkId = m_actor_armor;
            }
            if (command->actor.type == ActorConditionType::State) {
              command->actor.checkId = m_actor_state;
            }
          } else if (command->type == ConditionType::Enemy) {
            command->enemy.id = m_enemy_selection;
            command->enemy.type = static_cast<EnemyConditionType>(m_enemy_sub_selection);
            if (command->enemy.type == EnemyConditionType::State) {
              command->enemy.stateId = m_enemy_sub_state;
            } else {
              command->enemy.stateId = 0;
            }
          } else if (command->type == ConditionType::Character) {
            command->character.id = m_character_selection;
            command->character.facing = static_cast<Direction>((m_character_direction * 2));
          } else if (command->type == ConditionType::Vehicle) {
            command->vehicle.id = static_cast<VehicleType>(m_vehicle_selection);
          } else if (command->type == ConditionType::Gold) {
            command->gold.type = static_cast<GoldComparisonType>(m_gold_operation);
            command->gold.value = m_gold_selection;
          } else if (command->type == ConditionType::Item) {
            command->item.id = m_item_selection;
          } else if (command->type == ConditionType::Weapon) {
            command->equip.equipId = m_weapon_selection;
            command->equip.includeEquipment = m_weapon_include;
          } else if (command->type == ConditionType::Armor) {
            command->equip.equipId = m_armor_selection;
            command->equip.includeEquipment = m_armor_include;
          } else if (command->type == ConditionType::Button) {
            command->button = static_cast<Button>(m_button_selection);
          } else if (command->type == ConditionType::Script) {
            command->script = m_script;
          }
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          setOpen(false);
        }
        ImGui::EndGroup();
      }
      ImGui::EndTabBar();
    }
    ImGui::EndPopup();
  }
  return std::make_tuple(!m_open, m_confirmed);
}
