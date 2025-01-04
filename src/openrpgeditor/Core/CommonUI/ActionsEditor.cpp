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
    if (ImGui::BeginTable("##orpg_actions_editor", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                          ImVec2{ImGui::GetContentRegionMax().x - 15, ImGui::GetContentRegionMax().y - 600})) {
      ImGui::TableSetupColumn("Skill");
      ImGui::TableSetupColumn("Condition");
      ImGui::TableSetupColumn("Rating");
      ImGui::TableHeadersRow();

      for (auto& action : *m_actions) {
        ImGui::PushID(&action);
        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
          if (ImGui::Selectable(std::format("{}##action_{}", Database::instance()->skillNameOrId(action.skillId), reinterpret_cast<uintptr_t>(&action)).c_str(), m_selectedAction == &action,
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
            ImGui::Text("Always");
            break;
          }
          case ActionCondition::Turn: {
            ImGui::Text("Turn %i + %i * X", static_cast<int>(action.conditionParam1), static_cast<int>(action.conditionParam2));
            break;
          }
          case ActionCondition::HP: {
            ImGui::Text("HP %i%% ~ %i%%", static_cast<int>(action.conditionParam1 * 100), static_cast<int>(action.conditionParam2 * 100));
            break;
          }
          case ActionCondition::MP: {
            ImGui::Text("MP %i%% ~ %i%%", static_cast<int>(action.conditionParam1 * 100), static_cast<int>(action.conditionParam2 * 100));
            break;
          }
          case ActionCondition::State: {
            ImGui::Text("State %s", Database::instance()->stateNameOrId(static_cast<int>(action.conditionParam1)).c_str());
            break;
          }
          case ActionCondition::Party_Level: {
            ImGui::Text("Party Level >= %i", static_cast<int>(action.conditionParam1));
            break;
          }
          case ActionCondition::Switch: {
            ImGui::Text("%s", Database::instance()->switchNameOrId(static_cast<int>(action.conditionParam1)).c_str());
            break;
          }
          default:
            // ImGui::Text("%i %g", effect.dataId, effect.value1);
            break;
          }
        }
        if (ImGui::TableNextColumn()) {
          ImGui::Text("%i", action.rating);
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
      ImGui::BeginChild("##orpg_actions_upper_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
      {
        ImGui::SeparatorText("Skill and Rating");
        ImGui::BeginGroup();
        {
          ImGui::Text("Skill:");
          ImGui::SameLine();
          ImGui::Dummy(ImVec2{160, 0});
          ImGui::SameLine();
          ImGui::Text("Rating:");
          ImGui::EndGroup();
        }
        ImGui::BeginGroup();
        {
          // Skill Button - Picker
          if (ImGui::Button(Database::instance()->skillNameOrId(m_selectedAction->skillId).c_str(), ImVec2{200, 0})) {
            m_skillPicker.emplace("Skill"sv, Database::instance()->skills.skills(), m_selectedAction->skillId);
            m_skillPicker->setOpen(true);
          }
          ImGui::SameLine();
          ImGui::SetNextItemWidth(150);
          int tmpInt = m_selectedAction->rating;
          if (ImGui::InputInt("##action_rating", &tmpInt)) {
            m_selectedAction->rating = tmpInt;
          }
          ImGui::SeparatorText("Conditions");
          ImGui::EndGroup();
        }
      }
      ImGui::EndChild();
      ImGui::BeginChild("##orpg_actions_left_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
      {

        if (ImGui::RadioButton("Always", m_selectedAction->conditionType == ActionCondition::Always)) {
          if (m_selectedAction->conditionType != ActionCondition::Always) {
            // TODO
          }
          m_selectedAction->conditionType = ActionCondition::Always;
        }
        if (ImGui::RadioButton("Turn", m_selectedAction->conditionType == ActionCondition::Turn)) {
          if (m_selectedAction->conditionType != ActionCondition::Turn) {
            m_selectedAction->conditionParam1 = 0;
            m_selectedAction->conditionParam2 = 0;
          }
          m_selectedAction->conditionType = ActionCondition::Turn;
        }
        if (ImGui::RadioButton("HP", m_selectedAction->conditionType == ActionCondition::HP)) {
          if (m_selectedAction->conditionType != ActionCondition::HP) {
            m_selectedAction->conditionParam1 = 0;
            m_selectedAction->conditionParam2 = 1;
          }
          m_selectedAction->conditionType = ActionCondition::HP;
        }
        // MP
        if (ImGui::RadioButton("MP", m_selectedAction->conditionType == ActionCondition::MP)) {
          if (m_selectedAction->conditionType != ActionCondition::MP) {
            m_selectedAction->conditionParam1 = 0;
            m_selectedAction->conditionParam2 = 1;
          }
          m_selectedAction->conditionType = ActionCondition::MP;
        }
        // State
        if (ImGui::RadioButton("State", m_selectedAction->conditionType == ActionCondition::State)) {
          if (m_selectedAction->conditionType != ActionCondition::State) {
            m_selectedAction->conditionParam1 = 1; // State id
            m_selectedAction->conditionParam2 = 0;
          }
          m_selectedAction->conditionType = ActionCondition::State;
        }
        // Party Level
        if (ImGui::RadioButton("Party Level", m_selectedAction->conditionType == ActionCondition::Party_Level)) {
          if (m_selectedAction->conditionType != ActionCondition::Party_Level) {
            m_selectedAction->conditionParam1 = 1;
            m_selectedAction->conditionParam2 = 0;
          }
          m_selectedAction->conditionType = ActionCondition::Party_Level;
        }
        // Switch
        if (ImGui::RadioButton("Switch", m_selectedAction->conditionType == ActionCondition::Switch)) {
          if (m_selectedAction->conditionType != ActionCondition::Switch) {
            m_selectedAction->conditionParam1 = 1;
            m_selectedAction->conditionParam2 = 0;
          }
          m_selectedAction->conditionType = ActionCondition::Switch;
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_actions_right_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
        {
          // Always
          // => No Widget
          ImGui::NewLine();
          ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 13.f);

          // Turn
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::Turn);
          {
            int tmpInt = m_selectedAction->conditionType == ActionCondition::Turn ? static_cast<int>(m_selectedAction->conditionParam1) : 0;
            ImGui::SetNextItemWidth(230);
            if (ImGui::InputInt("##action_turn_value1", &tmpInt)) {
              m_selectedAction->conditionParam1 = static_cast<float>(tmpInt);
            }
            ImGui::SameLine();
            ImGui::Text("~");
            ImGui::SameLine();
            int tmpInt2 = m_selectedAction->conditionType == ActionCondition::Turn ? static_cast<int>(m_selectedAction->conditionParam2) : 0;
            ImGui::SameLine();
            ImGui::SetNextItemWidth(230);
            if (ImGui::InputInt("##action_turn_value2", &tmpInt2)) {
              m_selectedAction->conditionParam2 = static_cast<float>(tmpInt2);
            }
          }
          ImGui::EndDisabled();
          // HP
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::HP);
          {
            int tmpInt = m_selectedAction->conditionType == ActionCondition::HP ? static_cast<int>(m_selectedAction->conditionParam1 / 100) : 0;
            ImGui::SetNextItemWidth(230);
            if (ImGui::SliderInt("##action_hp_value1", &tmpInt, 0, 100, m_selectedAction->conditionType == ActionCondition::HP ? "%d%%" : "")) {
              m_selectedAction->conditionParam1 = static_cast<float>(tmpInt / 100.0);
            }
            ImGui::SameLine();
            ImGui::Text("~");
            ImGui::SameLine();
            int tmpInt2 = m_selectedAction->conditionType == ActionCondition::HP ? static_cast<int>(m_selectedAction->conditionParam2 * 100) : 0;
            ImGui::SetNextItemWidth(230);
            if (ImGui::SliderInt("##action_hp_value2", &tmpInt2, 0, 100, m_selectedAction->conditionType == ActionCondition::HP ? "%d%%" : "")) {
              m_selectedAction->conditionParam2 = static_cast<float>(tmpInt2 / 100.0);
            }
          }
          ImGui::EndDisabled();
          // MP
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::MP);
          {
            int tmpInt = m_selectedAction->conditionType == ActionCondition::MP ? static_cast<int>(m_selectedAction->conditionParam1 * 100) : 0;
            ImGui::SetNextItemWidth(230);
            if (ImGui::SliderInt("##action_mp_value1", &tmpInt, 0, 100, m_selectedAction->conditionType == ActionCondition::MP ? "%d%%" : "")) {
              m_selectedAction->conditionParam1 = static_cast<float>(tmpInt / 100.0);
            }
            ImGui::SameLine();
            ImGui::Text("~");
            ImGui::SameLine();
            int tmpInt2 = m_selectedAction->conditionType == ActionCondition::MP ? static_cast<int>(m_selectedAction->conditionParam2 * 100) : 0;
            ImGui::SameLine();
            ImGui::SetNextItemWidth(230);
            if (ImGui::SliderInt("##action_mp_value2", &tmpInt2, 0, 100, m_selectedAction->conditionType == ActionCondition::MP ? "%d%%" : "")) {
              m_selectedAction->conditionParam2 = static_cast<float>(tmpInt2 / 100.0);
            }
          }
          ImGui::EndDisabled();
          // State
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::State);
          {
            // State Button - Picker
            if (ImGui::Button(m_selectedAction->conditionType == ActionCondition::State ? Database::instance()->stateNameOrId(static_cast<int>(m_selectedAction->conditionParam1)).c_str()
                                                                                        : "##actions_skill_selection",
                              ImVec2{ImGui::GetContentRegionAvail().x, 0})) {
              m_statePicker.emplace("State"sv, Database::instance()->states.states(), m_selectedAction->conditionParam1);
              m_statePicker->setOpen(true);
            }
          }
          ImGui::EndDisabled();

          // Party Level
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::Party_Level);
          {
            int tmpInt = m_selectedAction->conditionType == ActionCondition::Party_Level ? static_cast<int>(m_selectedAction->conditionParam1) : 0;
            ImGui::SetNextItemWidth(230);
            if (ImGui::InputInt("##action_party_level_value1", &tmpInt)) {
              m_selectedAction->conditionParam1 = static_cast<float>(tmpInt);
            }
            ImGui::SameLine();
            ImGui::Text("or above");
          }
          ImGui::EndDisabled();

          // Switch
          ImGui::BeginDisabled(m_selectedAction->conditionType != ActionCondition::Switch);
          {
            // Switch Button - Picker
            if (ImGui::Button(m_selectedAction->conditionType == ActionCondition::Switch ? Database::instance()->switchNameOrId(static_cast<int>(m_selectedAction->conditionParam1)).c_str()
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
          m_selectedAction->conditionParam1 = static_cast<float>(m_statePicker->selection());
        }
        m_statePicker.reset();
      }
    }
    if (picker) {
      auto [closed, confirmed] = picker->draw();
      if (closed) {
        if (confirmed) {
          m_selectedAction->conditionParam1 = static_cast<float>(picker->selection());
        }
        picker.reset();
      }
    }
    ImGui::EndPopup();
  }
  m_updateTab = false;
}