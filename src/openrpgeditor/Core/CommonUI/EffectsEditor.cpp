#include "Core/CommonUI/EffectsEditor.hpp"
#include "Core/DatabaseEditor.hpp"

#include <format>

#include "imgui.h"

using namespace std::string_view_literals;

static constexpr auto EffectsEditorPopupId = "Effects Edit"sv;

template <>
inline int ObjectPicker<std::optional<CommonEvent>>::getId(const std::optional<CommonEvent>& value) {
  return value ? value->id() : 0;
}

static const std::string InvalidCommonEvent = "Invalid Common Event";
template <>
inline const std::string& ObjectPicker<std::optional<CommonEvent>>::getName(const std::optional<CommonEvent>& value) {
  return value ? value->name() : InvalidCommonEvent;
}

void EffectsEditor::draw(DatabaseEditor* dbEditor) {
  if (m_effects == nullptr) {
    return;
  }

  ImGui::BeginGroup();
  {
    ImGui::SeparatorText("Effects");
    if (ImGui::BeginTable("##orpg_effects_editor", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollY,
                          ImVec2{ImGui::GetContentRegionMax().x - 15, ImGui::GetContentRegionMax().y - 600})) {
      ImGui::TableSetupColumn("Type");
      ImGui::TableSetupColumn("Content");
      ImGui::TableHeadersRow();

      for (auto& effect : *m_effects) {
        ImGui::PushID(&effect);
        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
          if (ImGui::Selectable(std::format("{}##effect_{}", DecodeEnumName(effect.code), reinterpret_cast<uintptr_t>(&effect)).c_str(), m_selectedEffect == &effect,
                                ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            m_selectedEffect = &effect;
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_isNewEntry = false;
              m_updateTab = true;
              m_tempEffectAssigned = false;
              ImGui::OpenPopup(EffectsEditorPopupId.data());
            }
          }
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selectedEffect == &effect && m_deletingEffect == nullptr) {
            m_deletingEffect = &effect;
            m_selectedEffect = nullptr;
          }
          drawPopup(dbEditor);
        }
        if (ImGui::TableNextColumn()) {
          switch (effect.code) {
          case EffectCode::Recover_HP: {
            ImGui::Text("%i%% + %i", static_cast<int>(effect.value1 * 100), effect.value2);
            break;
          }
          case EffectCode::Recover_MP: {
            ImGui::Text("%i%% + %i", static_cast<int>(effect.value1 * 100), effect.value2);
            break;
          }
          case EffectCode::Gain_TP: {
            ImGui::Text("%i", static_cast<int>(effect.value1));
            break;
          }
          case EffectCode::Add_State: {
            ImGui::Text("%s %i%%", Database::instance()->stateNameOrId(effect.dataId).c_str(), static_cast<int>(effect.value1 * 100));
            break;
          }
          case EffectCode::Remove_State: {
            ImGui::Text("%s %i%%", Database::instance()->stateNameOrId(effect.dataId).c_str(), static_cast<int>(effect.value1 * 100));
            break;
          }
          case EffectCode::Add_Buff: {
            ImGui::Text("%s + %i turns", DecodeEnumName(static_cast<ParameterSource>(effect.dataId)).c_str(), static_cast<int>(effect.value1));
            break;
          }
          case EffectCode::Add_Debuff: {
            ImGui::Text("%s + %i turns", DecodeEnumName(static_cast<ParameterSource>(effect.dataId)).c_str(), static_cast<int>(effect.value1));
            break;
          }
          case EffectCode::Remove_Buff: {
            ImGui::Text("%s", DecodeEnumName(static_cast<ParameterSource>(effect.dataId)).c_str());
            break;
          }
          case EffectCode::Remove_Debuff: {
            ImGui::Text("%s", DecodeEnumName(static_cast<ParameterSource>(effect.dataId)).c_str());
            break;
          }
          case EffectCode::Special_Effect: {
            ImGui::Text("%s", DecodeEnumName(static_cast<SpecialEffectSource>(effect.dataId)).c_str());
            break;
          }
          case EffectCode::Grow: {
            ImGui::Text("%s + %i", DecodeEnumName(static_cast<ParameterSource>(effect.dataId)).c_str(), static_cast<int>(effect.value1));
            break;
          }
          case EffectCode::Learn_Skill: {
            ImGui::Text("%s", Database::instance()->skillNameOrId(effect.dataId).c_str());
            break;
          }
          case EffectCode::Common_Event: {
            ImGui::Text("%s", Database::instance()->commonEventNameOrId(effect.dataId).c_str());
            break;
          }
          default:
            ImGui::Text("%i %g", effect.dataId, effect.value1);
            break;
          }
        }

        ImGui::PopID();
      }

      /* Dummy entry for adding new effects */
      ImGui::TableNextRow();
      if (ImGui::TableNextColumn()) {
        if (ImGui::Selectable("##effects_editor_effect_dummy", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            m_tempEffect = Effect();
            m_isNewEntry = true;
            m_updateTab = true;
            m_tempEffectAssigned = false;
            m_selectedEffect = &m_tempEffect;
            ImGui::OpenPopup(EffectsEditorPopupId.data());
          }
        }
      }
      drawPopup(dbEditor);
      ImGui::EndTable();
    }
  }
  ImGui::EndGroup();

  if (m_deletingEffect) {
    std::erase_if(*m_effects, [&](const auto& trait) { return &trait == m_deletingEffect; });
    m_deletingEffect = nullptr;
  }
}

void EffectsEditor::drawPopup(DatabaseEditor* dbEditor) {
  if (m_selectedEffect == nullptr || m_effects == nullptr) {
    return;
  }
  if (ImGui::BeginPopupModal(EffectsEditorPopupId.data(), nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (!m_isNewEntry && !m_tempEffectAssigned) {
      // We're not a new entry so copy our values so we can restore them if we cancel
      m_tempEffect = *m_selectedEffect;
      m_tempEffectAssigned = true;
    }
    ImGui::BeginGroup();
    {
      if (ImGui::BeginTabBar("##orpg_effects_edit_tab_bar")) {
        // Tab 1 - Recover HP, Recover MP, Gain TP
        if (ImGui::BeginTabItem("Rate", nullptr,
                                m_selectedEffect->code >= EffectCode::Recover_HP && m_selectedEffect->code <= EffectCode::Gain_TP && m_updateTab ? ImGuiTabItemFlags_SetSelected : 0)) {
          ImGui::BeginChild("##orpg_effects_rate_left_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {

            if (ImGui::RadioButton("Recover HP", m_selectedEffect->code == EffectCode::Recover_HP)) {
              if (m_selectedEffect->code != EffectCode::Recover_HP) {
                m_selectedEffect->dataId = 0;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Recover_HP;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
            if (ImGui::RadioButton("Recover MP", m_selectedEffect->code == EffectCode::Recover_MP)) {
              if (m_selectedEffect->code != EffectCode::Recover_MP) {
                m_selectedEffect->dataId = 0;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Recover_MP;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
            if (ImGui::RadioButton("Gain TP", m_selectedEffect->code == EffectCode::Gain_TP)) {
              if (m_selectedEffect->code != EffectCode::Gain_TP) {
                m_selectedEffect->dataId = 0;
                m_selectedEffect->value1 = 0;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Gain_TP;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_effects_rate_right_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            // Recover HP
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Recover_HP);
            {
              int tmpInt = m_selectedEffect->code == EffectCode::Recover_HP ? static_cast<int>(m_selectedEffect->value1 * 100) : 0;
              if (ImGui::SliderInt("##effect_recover_hp_value", &tmpInt, -100, 100, m_selectedEffect->code == EffectCode::Recover_HP ? "%d%%" : "")) {
                m_selectedEffect->value1 = tmpInt / 100.0;
              }
              int tmpInt2 = m_selectedEffect->code == EffectCode::Recover_HP ? static_cast<int>(m_selectedEffect->value2) : 0;
              ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
              if (ImGui::InputInt("##effect_recover_hp_flat_value", &tmpInt2)) {
                m_selectedEffect->value2 = tmpInt;
              }
              ImGui::SameLine();
              ImGui::Text("(flat)");
            }
            ImGui::EndDisabled();
            // Recover MP
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Recover_MP);
            {
              int tmpInt = m_selectedEffect->code == EffectCode::Recover_MP ? static_cast<int>(m_selectedEffect->value1 * 100) : 0;
              if (ImGui::SliderInt("##effect_recover_mp_value", &tmpInt, -100, 100, m_selectedEffect->code == EffectCode::Recover_MP ? "%d%%" : "")) {
                m_selectedEffect->value1 = tmpInt / 100.0;
              }
              int tmpInt2 = m_selectedEffect->code == EffectCode::Recover_HP ? static_cast<int>(m_selectedEffect->value2) : 0;
              ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
              if (ImGui::InputInt("##effect_recover_mp_flat_value", &tmpInt2)) {
                m_selectedEffect->value2 = tmpInt;
              }
              ImGui::SameLine();
              ImGui::Text("(flat)");
            }
            ImGui::EndDisabled();
            // Gain TP
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Gain_TP);
            {
              int tmpInt = m_selectedEffect->code == EffectCode::Gain_TP ? static_cast<int>(m_selectedEffect->value1) : 0;
              ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
              if (ImGui::InputInt("##effect_gain_tp", &tmpInt)) {
                if (tmpInt > 100) {
                  tmpInt = 100;
                } else if (tmpInt < 0) {
                  tmpInt = 0;
                }
                m_selectedEffect->value1 = tmpInt;
              }
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }
        // Tab 2 - Add State, Remove State
        if (ImGui::BeginTabItem("State", nullptr,
                                m_selectedEffect->code >= EffectCode::Add_State && m_selectedEffect->code <= EffectCode::Remove_State && m_updateTab ? ImGuiTabItemFlags_SetSelected : 0)) {
          ImGui::BeginChild("##orpg_effects_state_left_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {

            if (ImGui::RadioButton("Add State", m_selectedEffect->code == EffectCode::Add_State)) {
              if (m_selectedEffect->code != EffectCode::Add_State) {
                m_selectedEffect->dataId = 1;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Add_State;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
            if (ImGui::RadioButton("Remove State", m_selectedEffect->code == EffectCode::Remove_State)) {
              if (m_selectedEffect->code != EffectCode::Remove_State) {
                m_selectedEffect->dataId = 1;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Remove_State;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_effects_state_right_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            // Add State
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Add_State);
            {
              // State Button - Picker
              if (ImGui::Button(m_selectedEffect->code == EffectCode::Add_State ? Database::instance()->stateNameOrId(m_selectedEffect->dataId).c_str() : "##effects_state_add_selection",
                                ImVec2{ImGui::GetContentRegionAvail().x, 0})) {
                m_statePicker.emplace("States"sv, Database::instance()->states.states(), m_selectedEffect->dataId);
                m_statePicker->setOpen(true);
              }
              // State %
              ImGui::SetNextItemWidth(250 * App::DPIHandler::get_ui_scale());
              int tmpInt = m_selectedEffect->code == EffectCode::Add_State ? static_cast<int>(m_selectedEffect->value1 * 100) : 0;
              if (ImGui::SliderInt("##effect_state_add_value", &tmpInt, 0, 100, m_selectedEffect->code == EffectCode::Add_State ? "%d%%" : "")) {
                m_selectedEffect->value1 = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();

            // Remove State
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Remove_State);
            {
              // State Button - Picker
              if (ImGui::Button(m_selectedEffect->code == EffectCode::Remove_State ? Database::instance()->stateNameOrId(m_selectedEffect->dataId).c_str() : "##effects_state_remove_selection",
                                ImVec2{ImGui::GetContentRegionAvail().x, 0})) {
                m_statePicker.emplace("States"sv, Database::instance()->states.states(), m_selectedEffect->dataId);
                m_statePicker->setOpen(true);
              }
              // State %;
              ImGui::SetNextItemWidth(250 * App::DPIHandler::get_ui_scale());
              int tmpInt = m_selectedEffect->code == EffectCode::Remove_State ? static_cast<int>(m_selectedEffect->value1 * 100) : 0;
              if (ImGui::SliderInt("##effect_state_remove_value", &tmpInt, 0, 100, m_selectedEffect->code == EffectCode::Remove_State ? "%d%%" : "")) {
                m_selectedEffect->value1 = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }

        // Tab 3 - Add Buff, Add Debuff, Remove Buff, Remove Debuff

        if (ImGui::BeginTabItem("Params", nullptr,
                                m_selectedEffect->code >= EffectCode::Add_Buff && m_selectedEffect->code <= EffectCode::Remove_Debuff && m_updateTab ? ImGuiTabItemFlags_SetSelected : 0)) {
          ImGui::BeginChild("##orpg_effects_params_left_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {

            if (ImGui::RadioButton("Add Buff", m_selectedEffect->code == EffectCode::Add_Buff)) {
              if (m_selectedEffect->code != EffectCode::Add_Buff) {
                m_selectedEffect->dataId = 0;
                m_selectedEffect->value1 = 5;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Add_Buff;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.f);
            if (ImGui::RadioButton("Add Debuff", m_selectedEffect->code == EffectCode::Add_Debuff)) {
              if (m_selectedEffect->code != EffectCode::Add_Debuff) {
                m_selectedEffect->dataId = 0;
                m_selectedEffect->value1 = 5;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Add_Debuff;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.f);
            if (ImGui::RadioButton("Remove Buff", m_selectedEffect->code == EffectCode::Remove_Buff)) {
              if (m_selectedEffect->code != EffectCode::Remove_Buff) {
                m_selectedEffect->dataId = 0;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Remove_Buff;
            }
            ImGui::Spacing();
            // ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.f);
            if (ImGui::RadioButton("Remove Debuff", m_selectedEffect->code == EffectCode::Remove_Debuff)) {
              if (m_selectedEffect->code != EffectCode::Remove_Debuff) {
                m_selectedEffect->dataId = 0;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Remove_Debuff;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_effects_params_right_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            // Add_Buff
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Add_Buff);
            {
              if (ImGui::BeginCombo("##effect_addbuff_combo", m_selectedEffect->code == EffectCode::Add_Buff ? DecodeEnumName(static_cast<ParameterSource>(m_selectedEffect->dataId)).c_str() : "")) {
                for (auto v : magic_enum::enum_values<ParameterSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<ParameterSource>(m_selectedEffect->dataId) == v)) {
                    m_selectedEffect->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt = m_selectedEffect->code == EffectCode::Add_Buff ? m_selectedEffect->value1 : 0;
              ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
              if (ImGui::InputInt("##effect_addbuff_value", &tmpInt)) {
                if (tmpInt > 1000) {
                  tmpInt = 1000;
                } else if (tmpInt < 1) {
                  tmpInt = 1;
                }
                m_selectedEffect->value1 = tmpInt;
              }
              ImGui::SameLine();
              ImGui::Text("turns");
            }
            ImGui::EndDisabled();
            // Add Debuff
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Add_Debuff);
            {
              if (ImGui::BeginCombo("##effect_adddebuff_combo",
                                    m_selectedEffect->code == EffectCode::Add_Debuff ? DecodeEnumName(static_cast<ParameterSource>(m_selectedEffect->dataId)).c_str() : "")) {
                for (auto v : magic_enum::enum_values<ParameterSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<ParameterSource>(m_selectedEffect->dataId) == v)) {
                    m_selectedEffect->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt = m_selectedEffect->code == EffectCode::Add_Debuff ? m_selectedEffect->value1 : 0;
              ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
              if (ImGui::InputInt("##effect_adddebuff_value", &tmpInt)) {
                if (tmpInt > 1000) {
                  tmpInt = 1000;
                } else if (tmpInt < 1) {
                  tmpInt = 1;
                }
                m_selectedEffect->value1 = tmpInt;
              }
              ImGui::SameLine();
              ImGui::Text("turns");
            }
            ImGui::EndDisabled();
            // Remove Buff
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Remove_Buff);
            {
              if (ImGui::BeginCombo("##effect_removebuff_combo",
                                    m_selectedEffect->code == EffectCode::Remove_Buff ? DecodeEnumName(static_cast<ParameterSource>(m_selectedEffect->dataId)).c_str() : "")) {
                for (auto v : magic_enum::enum_values<ParameterSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<ParameterSource>(m_selectedEffect->dataId) == v)) {
                    m_selectedEffect->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            // Remove Debuff
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Remove_Debuff);
            {
              if (ImGui::BeginCombo("##effect_removedebuff_combo",
                                    m_selectedEffect->code == EffectCode::Remove_Debuff ? DecodeEnumName(static_cast<ParameterSource>(m_selectedEffect->dataId)).c_str() : "")) {
                for (auto v : magic_enum::enum_values<ParameterSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<ParameterSource>(m_selectedEffect->dataId) == v)) {
                    m_selectedEffect->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }

        // Tab 4 - Special Effect, Grow, Learn Skill, Common Event

        if (ImGui::BeginTabItem("Other", nullptr,
                                m_selectedEffect->code >= EffectCode::Special_Effect && m_selectedEffect->code <= EffectCode::Common_Event && m_updateTab ? ImGuiTabItemFlags_SetSelected : 0)) {
          ImGui::BeginChild("##orpg_effects_params_left_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {

            if (ImGui::RadioButton("Special Effect", m_selectedEffect->code == EffectCode::Special_Effect)) {
              if (m_selectedEffect->code != EffectCode::Special_Effect) {
                m_selectedEffect->dataId = 0;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Special_Effect;
            }
            ImGui::Spacing();
            if (ImGui::RadioButton("Add Grow", m_selectedEffect->code == EffectCode::Grow)) {
              if (m_selectedEffect->code != EffectCode::Grow) {
                m_selectedEffect->dataId = 0;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Grow;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3.f);
            if (ImGui::RadioButton("Learn Skill", m_selectedEffect->code == EffectCode::Learn_Skill)) {
              if (m_selectedEffect->code != EffectCode::Learn_Skill) {
                m_selectedEffect->dataId = 1;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Learn_Skill;
            }
            ImGui::Spacing();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3.f);
            if (ImGui::RadioButton("Common Event", m_selectedEffect->code == EffectCode::Common_Event)) {
              if (m_selectedEffect->code != EffectCode::Common_Event) {
                m_selectedEffect->dataId = 1;
                m_selectedEffect->value1 = 1;
                m_selectedEffect->value2 = 0;
              }
              m_selectedEffect->code = EffectCode::Common_Event;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_effects_params_right_child", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            // Special_Effect
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Special_Effect);
            {
              if (ImGui::BeginCombo("##effect_special_effect_combo",
                                    m_selectedEffect->code == EffectCode::Special_Effect ? DecodeEnumName(static_cast<SpecialEffectSource>(m_selectedEffect->dataId)).c_str() : "")) {
                for (auto v : magic_enum::enum_values<SpecialEffectSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<SpecialEffectSource>(m_selectedEffect->dataId) == v)) {
                    m_selectedEffect->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            // Grow
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Grow);
            {
              if (ImGui::BeginCombo("##effect_grow_combo", m_selectedEffect->code == EffectCode::Grow ? DecodeEnumName(static_cast<ParameterSource>(m_selectedEffect->dataId)).c_str() : "")) {
                for (auto v : magic_enum::enum_values<ParameterSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(), static_cast<ParameterSource>(m_selectedEffect->dataId) == v)) {
                    m_selectedEffect->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt = m_selectedEffect->code == EffectCode::Grow ? m_selectedEffect->value1 : 0;
              ImGui::SetNextItemWidth(230 * App::DPIHandler::get_ui_scale());
              if (ImGui::InputInt("##effect_grow_value", &tmpInt)) {
                if (tmpInt > 1000) {
                  tmpInt = 1000;
                } else if (tmpInt < 1) {
                  tmpInt = 1;
                }
                m_selectedEffect->value1 = tmpInt;
              }
            }
            ImGui::EndDisabled();
            // Learn Skill
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Learn_Skill);
            {
              // Skill Button - Picker
              if (ImGui::Button(m_selectedEffect->code == EffectCode::Learn_Skill ? Database::instance()->skillNameOrId(m_selectedEffect->dataId).c_str() : "##effects_learn_skill_selection",
                                ImVec2{ImGui::GetContentRegionAvail().x, 0})) {
                m_skillPicker.emplace("Skill"sv, Database::instance()->skills.skills(), m_selectedEffect->dataId);
                m_skillPicker->setOpen(true);
              }
            }
            ImGui::EndDisabled();
            // Common Event
            ImGui::BeginDisabled(m_selectedEffect->code != EffectCode::Common_Event);
            {
              // Common Event Button - Picker
              if (ImGui::Button(m_selectedEffect->code == EffectCode::Common_Event ? Database::instance()->commonEventNameOrId(m_selectedEffect->dataId).c_str() : "##effects_common_event_selection",
                                ImVec2{ImGui::GetContentRegionAvail().x, 0})) {

                m_commonEventPicker.emplace("Common Event"sv, Database::instance()->commonEvents.events(), m_selectedEffect->dataId);
                m_commonEventPicker->setOpen(true);
              }
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }
      }
      ImGui::EndTabBar();
      ImGui::EndGroup();
    }
    ImGui::Separator();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        if (m_isNewEntry) {
          m_effects->push_back(m_tempEffect);
        }
        m_isNewEntry = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        if (!m_isNewEntry) {
          // Restore values to their unmodified state;
          *m_selectedEffect = m_tempEffect;
        }
        m_isNewEntry = false;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();

    if (m_commonEventPicker) {
      if (auto [closed, confirmed] = m_commonEventPicker->draw(); closed) {
        if (confirmed) {
          m_selectedEffect->dataId = m_commonEventPicker->selection();
        }
        m_commonEventPicker.reset();
      }
    }

    if (m_skillPicker) {
      if (auto [closed, confirmed] = m_skillPicker->draw(); closed) {
        if (confirmed) {
          m_selectedEffect->dataId = m_skillPicker->selection();
        }
        m_skillPicker.reset();
      }
    }
    ImGui::EndPopup();
  }
  m_updateTab = false;
}