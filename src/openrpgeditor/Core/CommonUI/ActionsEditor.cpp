#include "Core/CommonUI/ActionsEditor.hpp"
#include "Core/DatabaseEditor.hpp"

#include <format>

#include "imgui.h"

using namespace std::string_view_literals;

static constexpr auto ActionsEditorPopupId = "Actions Edit"sv;

void ActionsEditor::draw(DatabaseEditor* dbEditor) {
  if (m_actions == nullptr) {
    return;
  }

  ImGui::BeginGroup();
  {
    ImGui::SeparatorText("Action Patterns");
    if (ImGui::BeginTable("##orpg_actions_editor", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollY,
                          ImVec2{ImGui::GetContentRegionMax().x - 15, ImGui::GetContentRegionMax().y - 600})) {
      ImGui::TableSetupColumn("Type");
      ImGui::TableSetupColumn("Content");
      ImGui::TableHeadersRow();

      for (auto& action : *m_actions) {
        ImGui::PushID(&action);
        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
          if (ImGui::Selectable(std::format("{}##action_{}", DecodeEnumName(action.conditionType), reinterpret_cast<uintptr_t>(&action)).c_str(), m_selectedAction == &action,
                                ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            m_selectedAction = &action;
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_isNewEntry = false;
              m_updateTab = true;
              m_tempEffectAssigned = false;
              ImGui::OpenPopup(ActionsEditorPopupId.data());
            }
          }
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selectedAction == &action && m_deletingAction == nullptr) {
            m_deletingAction = &action;
            m_selectedAction = nullptr;
          }
          drawPopup(dbEditor);
        }
        if (ImGui::TableNextColumn()) {
          switch (action.conditionType) {
          case ActionCondition::Always: {
            // ImGui::Text("%i%% + %i", static_cast<int>(effect.value1 * 100), effect.value2);
            break;
          }
          case ActionCondition::Turn: {
            // ImGui::Text("%i%% + %i", static_cast<int>(effect.value1 * 100), effect.value2);
            break;
          }
          case ActionCondition::HP: {
            // ImGui::Text("%i", static_cast<int>(effect.value1));
            break;
          }
          case ActionCondition::MP: {
            // ImGui::Text("%s %i%%", Database::instance()->stateNameOrId(effect.dataId).c_str(), static_cast<int>(effect.value1 * 100));
            break;
          }
          case ActionCondition::State: {
            // ImGui::Text("%s %i%%", Database::instance()->stateNameOrId(effect.dataId).c_str(), static_cast<int>(effect.value1 * 100));
            break;
          }
          case ActionCondition::Party_Level: {
            // ImGui::Text("%s + %i turns", DecodeEnumName(static_cast<ParameterSource>(effect.dataId)).c_str(), static_cast<int>(effect.value1));
            break;
          }
          case ActionCondition::Switch: {
            // ImGui::Text("%s + %i turns", DecodeEnumName(static_cast<ParameterSource>(effect.dataId)).c_str(), static_cast<int>(effect.value1));
            break;
          }
          default:
            // ImGui::Text("%i %g", effect.dataId, effect.value1);
            break;
          }
        }

        ImGui::PopID();
      }

      /* Dummy entry for adding new actions */
      ImGui::TableNextRow();
      if (ImGui::TableNextColumn()) {
        if (ImGui::Selectable("##actions_editor_action_dummy", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            // m_tempAction = Action();
            m_isNewEntry = true;
            m_updateTab = true;
            m_tempEffectAssigned = false;
            m_selectedAction = &m_tempAction;
            ImGui::OpenPopup(ActionsEditorPopupId.data());
          }
        }
      }
      drawPopup(dbEditor);
      ImGui::EndTable();
    }
  }
  ImGui::EndGroup();

  if (m_deletingAction) {
    std::erase_if(*m_actions, [&](const auto& trait) { return &trait == m_deletingAction; });
    m_deletingAction = nullptr;
  }
}

void ActionsEditor::drawPopup(DatabaseEditor* dbEditor) {
  if (m_selectedAction == nullptr || m_actions == nullptr) {
    return;
  }
  if (ImGui::BeginPopupModal(ActionsEditorPopupId.data(), nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (!m_isNewEntry && !m_tempEffectAssigned) {
      // We're not a new entry so copy our values so we can restore them if we cancel
      m_tempAction = *m_selectedAction;
      m_tempEffectAssigned = true;
    }
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("##orpg_actions_rate_left_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
      {

        if (ImGui::RadioButton("Always", m_selectedAction->conditionType == ActionCondition::Always)) {
          if (m_selectedAction->conditionType != ActionCondition::Always) {
            // TODO
          }
          m_selectedAction->conditionType = ActionCondition::Always;
        }
        if (ImGui::RadioButton("Turn", m_selectedAction->conditionType == ActionCondition::Turn)) {
          if (m_selectedAction->conditionType != ActionCondition::Turn) {
            // TODO
          }
          m_selectedAction->conditionType = ActionCondition::Turn;
        }
        if (ImGui::RadioButton("HP", m_selectedAction->conditionType == ActionCondition::HP)) {
          if (m_selectedAction->conditionType != ActionCondition::HP) {
            // TODO
          }
          m_selectedAction->conditionType = ActionCondition::HP;
        }
        // MP
        if (ImGui::RadioButton("MP", m_selectedAction->conditionType == ActionCondition::MP)) {
          if (m_selectedAction->conditionType != ActionCondition::MP) {
            // TODO
          }
          m_selectedAction->conditionType = ActionCondition::MP;
        }
        // State
        if (ImGui::RadioButton("State", m_selectedAction->conditionType == ActionCondition::State)) {
          if (m_selectedAction->conditionType != ActionCondition::State) {
            // TODO
          }
          m_selectedAction->conditionType = ActionCondition::State;
        }
        // Party Level
        if (ImGui::RadioButton("Party Level", m_selectedAction->conditionType == ActionCondition::Party_Level)) {
          if (m_selectedAction->conditionType != ActionCondition::Party_Level) {
            // TODO
          }
          m_selectedAction->conditionType = ActionCondition::Party_Level;
        }
        // Switch
        if (ImGui::RadioButton("Switch", m_selectedAction->conditionType == ActionCondition::Switch)) {
          if (m_selectedAction->conditionType != ActionCondition::Switch) {
            // TODO
          }
          m_selectedAction->conditionType = ActionCondition::Switch;
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_effects_rate_right_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
        {
          // Always
          // => No Widget
          ImGui::NewLine();
          ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15.f);

          // Turn
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::Turn);
          {
            int tmpInt = m_selectedAction->conditionType == ActionCondition::Turn ? m_selectedAction->conditionParam1 : 0;
            ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
            if (ImGui::InputInt("##action_turn_value1", &tmpInt)) {
              m_selectedAction->conditionParam1 = tmpInt;
            }
            ImGui::SameLine();
            ImGui::Text("~");
            ImGui::SameLine();
            int tmpInt2 = m_selectedAction->conditionType == ActionCondition::Turn ? m_selectedAction->conditionParam2 : 0;
            ImGui::SameLine();
            ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
            if (ImGui::InputInt("##action_turn_value2", &tmpInt)) {
              m_selectedAction->conditionParam2 = tmpInt2;
            }
          }
          ImGui::EndDisabled();
          // HP
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::HP);
          {
            int tmpInt = m_selectedAction->conditionType == ActionCondition::HP ? m_selectedAction->conditionParam1 : 0;
            ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
            if (ImGui::InputInt("##action_hp_value1", &tmpInt)) {
              m_selectedAction->conditionParam1 = tmpInt;
            }
            ImGui::SameLine();
            ImGui::Text("~");
            ImGui::SameLine();
            int tmpInt2 = m_selectedAction->conditionType == ActionCondition::HP ? m_selectedAction->conditionParam2 : 0;
            ImGui::SameLine();
            ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
            if (ImGui::InputInt("##action_hp_value2", &tmpInt)) {
              m_selectedAction->conditionParam2 = tmpInt2;
            }
          }
          ImGui::EndDisabled();
          // MP
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::MP);
          {
            int tmpInt = m_selectedAction->conditionType == ActionCondition::MP ? m_selectedAction->conditionParam1 : 0;
            ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
            if (ImGui::InputInt("##action_mp_value1", &tmpInt)) {
              m_selectedAction->conditionParam1 = tmpInt;
            }
            ImGui::SameLine();
            ImGui::Text("~");
            ImGui::SameLine();
            int tmpInt2 = m_selectedAction->conditionType == ActionCondition::MP ? m_selectedAction->conditionParam2 : 0;
            ImGui::SameLine();
            ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
            if (ImGui::InputInt("##action_mp_value2", &tmpInt)) {
              m_selectedAction->conditionParam2 = tmpInt2;
            }
          }
          ImGui::EndDisabled();
          // State
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::State);
          {
            // State Button - Picker
            if (ImGui::Button(m_selectedAction->conditionType == ActionCondition::State ? Database::instance()->stateNameOrId(m_selectedAction->conditionParam1).c_str() : "##actions_skill_selection",
                              ImVec2{ImGui::GetContentRegionAvail().x, 0})) {
              m_statePicker.emplace("State"sv, Database::instance()->states.states(), m_selectedAction->conditionParam1);
              m_statePicker->setOpen(true);
            }
          }
          ImGui::EndDisabled();

          // Party Level
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::Party_Level);
          {
            int tmpInt = m_selectedAction->conditionType == ActionCondition::Party_Level ? m_selectedAction->conditionParam1 : 0;
            ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
            if (ImGui::InputInt("##action_party_level_value1", &tmpInt)) {
              m_selectedAction->conditionParam1 = tmpInt;
            }
            ImGui::SameLine();
            ImGui::Text("or above");
          }
          ImGui::EndDisabled();

          // Switch
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::Switch);
          {
            // Switch Button - Picker
            if (ImGui::Button(m_selectedAction->conditionType == ActionCondition::Switch ? Database::instance()->switchNameOrId(m_selectedAction->conditionParam1).c_str()
                                                                                         : "##actions_switch_selection",
                              ImVec2{ImGui::GetContentRegionAvail().x, 0})) {
              picker.emplace("Switches", Database::instance()->system.switches, m_selectedAction->conditionParam1);
              picker->setOpen(true);
            }
          }
          ImGui::EndDisabled();
        }
        ImGui::EndChild();
        ImGui::EndGroup();
      }
      ImGui::Separator();
      ImGui::BeginGroup();
      {
        if (ImGui::Button("OK")) {
          if (m_isNewEntry) {
            m_actions->push_back(m_tempAction);
          }
          m_isNewEntry = false;
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          if (!m_isNewEntry) {
            // Restore values to their unmodified state;
            *m_selectedAction = m_tempAction;
          }
          m_isNewEntry = false;
          ImGui::CloseCurrentPopup();
        }
      }
      ImGui::EndGroup();
    }

    if (m_skillPicker) {
      if (auto [closed, confirmed] = m_skillPicker->draw(); closed) {
        if (confirmed) {
          m_selectedAction->skillId = m_skillPicker->selection();
        }
        m_skillPicker.reset();
      }
    }
    if (m_statePicker) {
      if (auto [closed, confirmed] = m_statePicker->draw(); closed) {
        if (confirmed) {
          m_selectedAction->conditionParam1 = m_statePicker->selection();
        }
        m_statePicker.reset();
      }
    }
    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (closed) {
        if (confirmed) {
          m_selectedAction->conditionParam1 = picker->selection();
        }
        picker.reset();
      }
    }
    ImGui::EndPopup();
  }
  m_updateTab = false;
}