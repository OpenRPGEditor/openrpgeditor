
#include "Dialog_ConditionalBranch.hpp"
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "Core/Project.hpp"

std::tuple<bool, bool> Dialog_ConditionalBranch::draw() {

  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{500, 400} * App::DPIHandler::get_ui_scale());
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)) {
    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (confirmed) {
        if (m_picker_type == 0) {
          m_switch_id = picker->selection();
        } else if (m_picker_type == 2) {
          m_sub_variable_id = picker->selection();
        } else {
          m_variable_id = picker->selection();
        }
        picker.reset();
      }
    }

    if (actor_picker) {
      auto [closed, confirmed] = actor_picker->draw();
      if (confirmed) {
        m_actor_sub_selection = actor_picker->selection();
        actor_picker.reset();
      }
    }
    if (class_picker) {
      auto [closed, confirmed] = class_picker->draw();
      if (confirmed) {
        m_actor_class = class_picker->selection();
        class_picker.reset();
      }
    }
    if (skill_picker) {
      auto [closed, confirmed] = skill_picker->draw();
      if (confirmed) {
        m_actor_skill = skill_picker->selection();
        skill_picker.reset();
      }
    }
    if (weapon_picker) {
      auto [closed, confirmed] = weapon_picker->draw();
      if (confirmed) {
        m_actor_weapon = weapon_picker->selection();
        weapon_picker.reset();
      }
    }
    if (armor_picker) {
      auto [closed, confirmed] = armor_picker->draw();
      if (confirmed) {
        m_actor_armor = armor_picker->selection();
        armor_picker.reset();
      }
    }
    if (state_picker) {
      auto [closed, confirmed] = state_picker->draw();
      if (confirmed) {
        m_actor_state = state_picker->selection();
        state_picker.reset();
      }
    }
    if (ImGui::BeginTabBar("##conditional_branch_dialog")) {

      // ImVec2 size = ImVec2{((ImGui::GetContentRegionAvail().x / 2) / 2) + App::DPIHandler::scale_value(50), 0};

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
          std::string text =
              m_conditionType != 0
                  ? "##conditional_switch_empty"
                  : (m_switch_id == 0 ? "" : std::format("{:04} ", m_switch_id) + m_project->switche(m_switch_id));
          ImGui::PushID("##conditional_switch_id");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x + 50) - (16 * App::DPIHandler::get_ui_scale()));
          ImGui::BeginDisabled(m_conditionType != 0);
          if (ImGui::Button(text.c_str(), ImVec2{App::DPIHandler::scale_value(200), 0})) {
            m_picker_type = 0;
            picker.emplace("Switches", m_project->system().switches);
          }
          ImGui::PopID();
          ImGui::SameLine();
          ImGui::Text("is");
          ImGui::SameLine();
          ImGui::PushItemWidth((App::DPIHandler::scale_value(100)));
          if (ImGui::BeginCombo("##conditional_switch_sub_selection",
                                DecodeEnumName(magic_enum::enum_value<ValueControl>(m_switch_value)).c_str())) {
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
          text = m_conditionType != 1 ? "##conditional_variable_id"
                                      : std::format("{:04} ", m_variable_id) + m_project->variable(m_variable_id);
          ImGui::PushID("##controlvariable_id2");
          if (ImGui::Button(text.c_str(), ImVec2{App::DPIHandler::scale_value(200), 0})) {
            m_picker_type = 1;
            picker.emplace("Variables", m_project->system().variables);
          }
          ImGui::PopID();

          ImGui::SameLine();
          ImGui::PushItemWidth((App::DPIHandler::scale_value(100)));
          if (ImGui::BeginCombo(
                  "##conditional_variable_sub_selection",
                  DecodeEnumName(magic_enum::enum_value<VariableComparisonType>(m_variable_value)).c_str())) {
            for (auto& dir : magic_enum::enum_values<VariableComparisonType>()) {
              bool is_selected = m_variable_value == magic_enum::enum_index(dir).value();
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(dir)).c_str(), is_selected)) {
                m_variable_value = magic_enum::enum_index(dir).value();
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
            ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
            if (ImGui::InputInt("##conditional_var_sub_constant", &m_sub_constant)) {
              if (m_sub_constant > 9999)
                m_sub_constant = 9999;
              if (m_sub_constant < 0)
                m_sub_constant = 0;
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_variable_subSource != 1);
            ImGui::PushID("##conditional_var_sub_variable");
            if (ImGui::Button(
                    m_variable_subSource != 1 ? "" : Database::Instance->variableNameAndId(m_sub_variable_id).c_str(),

                    ImVec2{(App::DPIHandler::scale_value(200)), 0})) {
              m_picker_type = 2;
              picker.emplace("Variables", m_project->system().variables);
            }
            ImGui::PopID();
            ImGui::EndDisabled();
            ImGui::EndGroup();
          }
          ImGui::EndDisabled();
          // Self Switches
          ImGui::BeginDisabled(m_conditionType != 2);
          ImGui::SetNextItemWidth(120 * App::DPIHandler::get_ui_scale());
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
          ImGui::PushItemWidth((App::DPIHandler::scale_value(100)));
          if (ImGui::BeginCombo("##conditional_selfswitch_sub_selection",
                                DecodeEnumName(magic_enum::enum_value<ValueControl>(m_selfSw_value)).c_str())) {
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
          ImGui::SetNextItemWidth(120 * App::DPIHandler::get_ui_scale());
          if (ImGui::BeginCombo(
                  "##conditional_selfswitch_sub_comparison",
                  DecodeEnumName(magic_enum::enum_value<TimerComparisonType>(m_timer_operation)).c_str())) {
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
            ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
            if (ImGui::InputInt("min", &m_timer_min)) {
              if (m_timer_min > 99) {
                m_timer_min = 99;
              }
              if (m_timer_min < 0) {
                m_timer_min = 0;
              }
            }
            ImGui::SetNextItemWidth(App::DPIHandler::scale_value(100));
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
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(25));
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::BeginGroup();
        {
          ImGui::PushID("##conditional_actor_condition");
          if (ImGui::Button(m_conditionType != 4
                                ? "" : (std::format("{:04} ", m_actor_selection) + Database::Instance->actorName(m_actor_selection)).c_str(),
                            {(App::DPIHandler::scale_value(160)), 0})) {

            actor_picker = ObjectPicker<Actor>("Actor"sv, Database::Instance->actors.actorList(), 0);
          }
          ImGui::PopID();
          ImGui::BeginDisabled(m_conditionType != 4);
          ImGui::RadioButton("In the party", &m_actor_sub_selection, 0);
          ImGui::EndDisabled();
          ImGui::EndGroup();
        }
        ImGui::SetCursorPosX(cursorPos.x);
        ImGui::BeginGroup(); {
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
        ImGui::BeginGroup(); {
          ImGui::BeginDisabled(m_actor_sub_selection != 1);
          ImGui::SetNextItemWidth(App::DPIHandler::scale_value(180));
          ImGui::InputText("##conditional_actor_nameinput", &m_actor_selection_nameinput);
          ImGui::EndDisabled();

          // Class Button
          ImGui::BeginDisabled(m_actor_sub_selection != 2);
          ImGui::PushID("##change_class_selection");
          if (ImGui::Button(m_actor_sub_selection != 2 ? "" : Database::Instance->className(m_actor_class).c_str(), ImVec2{(App::DPIHandler::scale_value(180)), 0})) {
            class_picker = ObjectPicker<Class>("Class"sv, Database::Instance->classes.classes(), 0);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          // Skill Button
          ImGui::BeginDisabled(m_actor_sub_selection != 3);
          ImGui::PushID("##change_skill_selection");
          if (ImGui::Button(
                  m_actor_sub_selection != 3 ? "" : (std::format("{:04} ", m_actor_skill) + Database::Instance->skillNameOrId(m_actor_skill)).c_str(),
                  ImVec2{(App::DPIHandler::scale_value(180)), 0})) {
            skill_picker = ObjectPicker<Skill>("Skill"sv, Database::Instance->skills.skills(), 0);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          // Weapons Button
          ImGui::BeginDisabled(m_actor_sub_selection != 4);
          ImGui::PushID("##change_weapons_selection");
          if (ImGui::Button(
                  m_actor_sub_selection != 4 ? "" : Database::Instance->weaponNameOrId(m_actor_weapon).c_str(),
                  {(App::DPIHandler::scale_value(180)), 0})) {
            weapon_picker = ObjectPicker<Weapon>("Weapon"sv, Database::Instance->weapons.weaponList(), 0);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          // Armors Button
          ImGui::BeginDisabled(m_actor_sub_selection != 5);
          ImGui::PushID("##change_armors_selection");
          if (ImGui::Button(m_actor_sub_selection != 5 ? "" : Database::Instance->armorNameOrId(m_actor_armor).c_str(), {(App::DPIHandler::scale_value(180)), 0})) {
            armor_picker = ObjectPicker<Armor>("Armor"sv, Database::Instance->armors.armorList(), 0);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          // State Button
          ImGui::BeginDisabled(m_actor_sub_selection != 6);
          ImGui::PushID("##change_state_selection");
          if (ImGui::Button(
                  m_actor_sub_selection != 6 ? "" : Database::Instance->stateNameOrId(m_actor_state).c_str(),
                  {(App::DPIHandler::scale_value(180)), 0})) {
            m_state_type = 0;
            state_picker = ObjectPicker<State>("States"sv, Database::Instance->states.states(), 0);
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
          ImGui::RadioButton("Vehicle", &m_conditionType, 7);
          ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImGui::BeginDisabled(m_conditionType != 5);

          ImGui::SetNextItemWidth(180 * App::DPIHandler::get_ui_scale());
          if (ImGui::BeginCombo("##gamedata_enemy_list", (m_conditionType != 5 ? "" : "#" + std::to_string(m_enemy_selection) + " " + Database::Instance->troopMemberName(0, m_enemy_selection)).c_str())) {
            for (int i = 1; i < 9; ++i) {
              if (ImGui::Selectable(("#" + std::to_string(i) + " " + Database::Instance->troopMemberName(0, i)).c_str(), i == m_enemy_selection)) {
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
          if (ImGui::Button(
                  m_enemy_sub_selection != 1 ? "" : Database::Instance->stateNameOrId(m_actor_state).c_str(),
                  {(App::DPIHandler::scale_value(180)), 0})) {
            m_state_type = 1;
            state_picker = ObjectPicker<State>("States"sv, Database::Instance->states.states(), 0);
          }
          ImGui::PopID();
          ImGui::EndDisabled();

          ImGui::EndDisabled();

          ImGui::SetCursorPosY(ImGui::GetCursorPosY() + App::DPIHandler::scale_value(10));
          ImGui::BeginDisabled(m_conditionType != 6);
          ImGui::SetNextItemWidth(180 * App::DPIHandler::get_ui_scale());
          if (ImGui::BeginCombo("##gamedata_character",
                                m_conditionType != 6 ? "" : m_character_selection == -1 ? "Player" : m_character_selection == 0 ? "This Event" : ("EV" + std::format("{:03} ", m_project->events().at(m_character_selection)->id)).c_str())) {

            if (ImGui::Selectable("Player", m_character_selection == -1)) {
              m_character_selection = -1;
              ImGui::SetItemDefaultFocus();
            }
            if (ImGui::Selectable("This Event", m_character_selection == 0)) {
              m_character_selection = 0;
              ImGui::SetItemDefaultFocus();
            }

            for (auto& dataSource : m_project->events()) {
              if (!dataSource.has_value())
                continue;

              bool is_selected = (m_character_selection == dataSource->id);
              if (ImGui::Selectable(("EV" + std::format("{:03} ", dataSource->id)).c_str(), is_selected)) {
                m_character_selection = dataSource->id;
                if (is_selected)
                  ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
          ImGui::Text("is facing");
          ImGui::SameLine();
          ImGui::PushItemWidth((App::DPIHandler::scale_value(75)));
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

          ImGui::BeginDisabled(m_conditionType != 7);
          ImGui::SetCursorPosY(ImGui::GetCursorPosY() + App::DPIHandler::scale_value(15));
          ImGui::SetNextItemWidth(180 * App::DPIHandler::get_ui_scale());
          if (ImGui::BeginCombo("##vehicle_location_selection", m_conditionType != 7 ? "" : DecodeEnumName(magic_enum::enum_value<VehicleType>(m_vehicle_selection)).c_str())) {
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
          ImGui::RadioButton("Gold", &m_conditionType, 8);
          ImGui::RadioButton("Item", &m_conditionType, 9);
          ImGui::RadioButton("Weapon", &m_conditionType, 10);
          ImGui::RadioButton("Armor", &m_conditionType, 11);
          ImGui::RadioButton("Button", &m_conditionType, 12);
          ImGui::RadioButton("Script", &m_conditionType, 13);
          ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        { ImGui::EndGroup(); }
        ImGui::EndTabItem();
      }
      ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - App::DPIHandler::scale_value(30));
      ImGui::Separator();
      ImGui::Checkbox("Create Else Branch", &m_elseBranch);
      ImGui::SetCursorPos(ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(105),
                                 ImGui::GetContentRegionMax().y - App::DPIHandler::scale_value(25)});
      ImGui::BeginGroup();
      {
        if (ImGui::Button("OK")) {
          m_confirmed = true;
          ImGui::CloseCurrentPopup();
          SetOpen(false);
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          SetOpen(false);
        }
        ImGui::EndGroup();
      }

      ImGui::EndPopup();
    }
  }
  return std::make_tuple(!m_open, m_confirmed);
}
