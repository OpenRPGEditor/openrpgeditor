#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor.hpp"

#include <format>

#include "imgui.h"
#include "imgui_internal.h"
#include "Core/DPIHandler.hpp"

using namespace std::string_view_literals;

static constexpr std::string_view TraitsEditorPopupId = "Traits Edit"sv;

void TraitsEditor::draw(DatabaseEditor* dbEditor) {
  if (m_traits == nullptr) {
    return;
  }

  ImGui::BeginGroup();
  {
    ImGui::SeparatorText("Traits");
    if (ImGui::BeginTable("##orpg_traits_editor", 2,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit |
                              ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollY,
                          ImVec2{ImGui::GetContentRegionMax().x - 15, ImGui::GetContentRegionMax().y - 600})) {
      ImGui::TableSetupColumn("Type");
      ImGui::TableSetupColumn("Content");
      ImGui::TableHeadersRow();

      for (auto& trait : *m_traits) {
        ImGui::PushID(&trait);
        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
          if (ImGui::Selectable(
                  std::format("{}##trait_{}", DecodeEnumName(trait.code), reinterpret_cast<uintptr_t>(&trait)).c_str(),
                  m_selectedTrait == &trait,
                  ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            m_selectedTrait = &trait;
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_isNewEntry = false;
              m_updateTab = true;
              m_tempTraitAssigned = false;
              ImGui::OpenPopup(TraitsEditorPopupId.data());
            }
          }
          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selectedTrait == &trait && m_deletingTrait == nullptr) {
            m_deletingTrait = &trait;
            m_selectedTrait = nullptr;
          }
          drawPopup(dbEditor);
        }

        if (ImGui::TableNextColumn()) {
          switch (trait.code) {
          case TraitCode::Element_Rate: {
            ImGui::Text("%s * %i%%", Database::Instance->equipTypeNameOrId(trait.dataId).c_str(),
                        static_cast<int>(trait.value * 100));
            break;
          }
          case TraitCode::Ex_daa_Parameter: {
            ImGui::Text("%s + %i%%", DecodeEnumName(static_cast<EXParameterSource>(trait.dataId)).c_str(),
                        static_cast<int>(trait.value * 100));
            break;
          }
          case TraitCode::Sp_daa_Parameter:
            ImGui::Text("%s * %i%%", DecodeEnumName(static_cast<SPParameterSource>(trait.dataId)).c_str(),
                        static_cast<int>(trait.value * 100));
            break;
          case TraitCode::Parameter:
            ImGui::Text("%s * %i%%", DecodeEnumName(static_cast<ParameterSource>(trait.dataId)).c_str(),
                        static_cast<int>(trait.value * 100));
            break;
          case TraitCode::Debuff_Rate:
            ImGui::Text("%s * %i%%", DecodeEnumName(static_cast<DebuffSource>(trait.dataId)).c_str(),
                        static_cast<int>(trait.value * 100));
            break;
          case TraitCode::Attack_State:
          case TraitCode::State_Rate: {
            ImGui::Text("%s %c %i%%", Database::Instance->stateNameOrId(trait.dataId).c_str(),
                        trait.code == TraitCode::Attack_State ? '+' : '*', static_cast<int>(trait.value * 100));
            break;
          }
          case TraitCode::State_Resist: {
            ImGui::Text("%s", Database::Instance->stateNameOrId(trait.dataId).c_str());
            break;
          }
          case TraitCode::Attack_Element: {
            ImGui::Text("%s", Database::Instance->elementNameOrId(trait.dataId).c_str());
            break;
          }
          case TraitCode::Attack_Speed:
          case TraitCode::Attack_Times__plu_:
            ImGui::Text("%g", trait.value);
            break;
          case TraitCode::Add_Skill_Type:
          case TraitCode::Seal_Skill_Type: {
            ImGui::Text("%s", Database::Instance->skillTypeNameOrId(trait.dataId).c_str());
            break;
          }
          case TraitCode::Add_Skill:
          case TraitCode::Seal_Skill: {
            ImGui::Text("%s", Database::Instance->skillNameOrId(trait.dataId).c_str());
            break;
          }
          case TraitCode::Equip_Weapon: {
            ImGui::Text("%s", Database::Instance->weaponTypeNameOrId(trait.dataId).c_str());
            break;
          }
          case TraitCode::Equip_Armor: {
            ImGui::Text("%s", Database::Instance->armorTypeOrId(trait.dataId).c_str());
            break;
          }
          case TraitCode::Lock_Equip:
          case TraitCode::Seal_Equip: {
            ImGui::Text("%s", Database::Instance->equipTypeNameOrId(trait.dataId).c_str());
            break;
          }
          case TraitCode::Slot_Type:
            ImGui::Text("%s", DecodeEnumName(static_cast<SlotType>(trait.dataId)).c_str());
            break;
          case TraitCode::Action_Times__plu_:
            ImGui::Text("%i%%", static_cast<int>(trait.value) * 100);
            break;
          case TraitCode::Special_Flag:
            ImGui::Text("%s", DecodeEnumName(static_cast<SpecialFlag>(trait.dataId)).c_str());
            break;
          case TraitCode::Collapse_Effect:
            ImGui::Text("%s", DecodeEnumName(static_cast<CollapseEffect>(trait.dataId)).c_str());
            break;
          case TraitCode::Party_Ability:
            ImGui::Text("%s", DecodeEnumName(static_cast<PartyAbility>(trait.dataId)).c_str());
            break;
          default:
            ImGui::Text("%i %g", trait.dataId, trait.value);
            break;
          }
        }

        ImGui::PopID();
      }

      /* Dummy entry for adding new traits */
      ImGui::TableNextRow();
      if (ImGui::TableNextColumn()) {
        if (ImGui::Selectable("##traits_editor_trait_dummy", false,
                              ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            m_tempTrait = Trait();
            m_isNewEntry = true;
            m_updateTab = true;
            m_tempTraitAssigned = false;
            m_selectedTrait = &m_tempTrait;
            ImGui::OpenPopup(TraitsEditorPopupId.data());
          }
        }
      }
      drawPopup(dbEditor);
      ImGui::EndTable();
    }
  }
  ImGui::EndGroup();

  if (m_deletingTrait) {
    std::erase_if(*m_traits, [&](const auto& trait) { return &trait == m_deletingTrait; });
    m_deletingTrait = nullptr;
  }
}

void TraitsEditor::drawPopup(DatabaseEditor* dbEditor) {
  if (m_selectedTrait == nullptr || m_traits == nullptr) {
    return;
  }

  if (ImGui::BeginPopupModal(TraitsEditorPopupId.data(), nullptr,
                             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize)) {
    if (!m_isNewEntry && !m_tempTraitAssigned) {
      // We're not a new entry so copy our values so we can restore them if we cancel
      m_tempTrait = *m_selectedTrait;
      m_tempTraitAssigned = true;
    }
    ImGui::BeginGroup();
    {
      if (ImGui::BeginTabBar("##orpg_traits_edit_tab_bar")) {
        if (ImGui::BeginTabItem("Rate", nullptr,
                                m_selectedTrait->code >= TraitCode::Element_Rate &&
                                        m_selectedTrait->code <= TraitCode::State_Resist && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::BeginChild("##orpg_traits_rate_left_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            if (ImGui::RadioButton("Element Rate", m_selectedTrait->code == TraitCode::Element_Rate)) {
              if (m_selectedTrait->code != TraitCode::State_Resist) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 1;
              }
              m_selectedTrait->code = TraitCode::Element_Rate;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            if (ImGui::RadioButton("Debuff Rate", m_selectedTrait->code == TraitCode::Debuff_Rate)) {
              if (m_selectedTrait->code != TraitCode::Debuff_Rate) {
                m_selectedTrait->dataId = static_cast<int>(DebuffSource::Max_HP);
                m_selectedTrait->value = 1;
              }
              m_selectedTrait->code = TraitCode::Debuff_Rate;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            if (ImGui::RadioButton("State Rate", m_selectedTrait->code == TraitCode::State_Rate)) {
              if (m_selectedTrait->code != TraitCode::State_Rate) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 1;
              }
              m_selectedTrait->code = TraitCode::State_Rate;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            if (ImGui::RadioButton("State Resist", m_selectedTrait->code == TraitCode::State_Resist)) {
              if (m_selectedTrait->code != TraitCode::State_Resist) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::State_Resist;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_traits_rate_right_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Element_Rate);
            {
              const auto element = dbEditor->element(m_selectedTrait->dataId);
              if (ImGui::BeginCombo("##trait_element_rate_combo",
                                    m_selectedTrait->code == TraitCode::Element_Rate ? element->c_str() : "")) {
                for (int i = 1; i < dbEditor->elementsCount(); ++i) {
                  auto v = dbEditor->element(i);
                  if (ImGui::Selectable(v ? (" " + *v).c_str() : "##traits_empty_element_name",
                                        i == m_selectedTrait->dataId)) {
                    m_selectedTrait->dataId = i;
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt =
                  m_selectedTrait->code == TraitCode::Element_Rate ? static_cast<int>(m_selectedTrait->value * 100) : 0;
              if (ImGui::SliderInt("##trait_element_rate_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::Element_Rate ? "* %d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Debuff_Rate);
            {
              if (ImGui::BeginCombo("##trait_debuff_rate_combo",
                                    m_selectedTrait->code == TraitCode::Debuff_Rate
                                        ? DecodeEnumName(static_cast<DebuffSource>(m_selectedTrait->dataId)).c_str()
                                        : "")) {
                for (auto v : magic_enum::enum_values<DebuffSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(),
                                        static_cast<DebuffSource>(m_selectedTrait->dataId) == v)) {
                    m_selectedTrait->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt =
                  m_selectedTrait->code == TraitCode::Debuff_Rate ? static_cast<int>(m_selectedTrait->value * 100) : 0;
              if (ImGui::SliderInt("##trait_debuff_rate_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::Debuff_Rate ? "* %d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::State_Rate);
            {
              if (ImGui::Button(m_selectedTrait->code == TraitCode::State_Rate
                                    ? Database::Instance->stateNameOrId(m_selectedTrait->dataId).c_str()
                                    : "##trait_state_rate_selection",
                                ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_statePicker.emplace("States"sv, Database::Instance->states.states(), m_selectedTrait->dataId);
              }
              int tmpInt =
                  m_selectedTrait->code == TraitCode::State_Rate ? static_cast<int>(m_selectedTrait->value * 100) : 0;
              if (ImGui::SliderInt("##trait_state_rate_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::State_Rate ? "* %d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::State_Resist);
            {
              if (ImGui::Button(m_selectedTrait->code == TraitCode::State_Resist
                                    ? Database::Instance->stateNameOrId(m_selectedTrait->dataId).c_str()
                                    : "##trait_state_resist_selection",
                                ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_statePicker.emplace("States"sv, Database::Instance->states.states(), m_selectedTrait->dataId);
              }
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Param", nullptr,
                                m_selectedTrait->code >= TraitCode::Parameter &&
                                        m_selectedTrait->code <= TraitCode::Sp_daa_Parameter && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::BeginChild("##orpg_traits_param_left_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            if (ImGui::RadioButton("Parameter", m_selectedTrait->code == TraitCode::Parameter)) {
              if (m_selectedTrait->code != TraitCode::Parameter) {
                m_selectedTrait->dataId = static_cast<int>(ParameterSource::Max_HP);
                m_selectedTrait->value = 1;
              }
              m_selectedTrait->code = TraitCode::Parameter;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            if (ImGui::RadioButton("Ex-Parameter", m_selectedTrait->code == TraitCode::Ex_daa_Parameter)) {
              if (m_selectedTrait->code != TraitCode::Ex_daa_Parameter) {
                m_selectedTrait->dataId = static_cast<int>(EXParameterSource::Hit_Rate);
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Ex_daa_Parameter;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            if (ImGui::RadioButton("Sp-Parameter", m_selectedTrait->code == TraitCode::Sp_daa_Parameter)) {
              if (m_selectedTrait->code != TraitCode::Sp_daa_Parameter) {
                m_selectedTrait->dataId = static_cast<int>(SPParameterSource::Target_Rate);
                m_selectedTrait->value = 1;
              }
              m_selectedTrait->code = TraitCode::Sp_daa_Parameter;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_traits_param_right_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Parameter);
            {
              if (ImGui::BeginCombo("##trait_parameter_combo",
                                    m_selectedTrait->code == TraitCode::Parameter
                                        ? DecodeEnumName(static_cast<ParameterSource>(m_selectedTrait->dataId)).c_str()
                                        : "")) {
                for (auto v : magic_enum::enum_values<ParameterSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(),
                                        static_cast<ParameterSource>(m_selectedTrait->dataId) == v)) {
                    m_selectedTrait->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt =
                  m_selectedTrait->code == TraitCode::Parameter ? static_cast<int>(m_selectedTrait->value * 100) : 0;
              if (ImGui::SliderInt("##trait_parameter_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::Parameter ? "* %d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Ex_daa_Parameter);
            {
              if (ImGui::BeginCombo(
                      "##trait_exparameter_combo",
                      m_selectedTrait->code == TraitCode::Ex_daa_Parameter
                          ? DecodeEnumName(static_cast<EXParameterSource>(m_selectedTrait->dataId)).c_str()
                          : "")) {
                for (auto v : magic_enum::enum_values<EXParameterSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(),
                                        static_cast<EXParameterSource>(m_selectedTrait->dataId) == v)) {
                    m_selectedTrait->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt = m_selectedTrait->code == TraitCode::Ex_daa_Parameter
                               ? static_cast<int>(m_selectedTrait->value * 100)
                               : 0;
              if (ImGui::SliderInt("##trait_exparameter_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::Ex_daa_Parameter ? "+ %d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Sp_daa_Parameter);
            {
              if (ImGui::BeginCombo(
                      "##trait_spparameter_combo",
                      m_selectedTrait->code == TraitCode::Sp_daa_Parameter
                          ? DecodeEnumName(static_cast<SPParameterSource>(m_selectedTrait->dataId)).c_str()
                          : "")) {
                for (auto v : magic_enum::enum_values<SPParameterSource>()) {
                  if (ImGui::Selectable(DecodeEnumName(v).c_str(),
                                        static_cast<SPParameterSource>(m_selectedTrait->dataId) == v)) {
                    m_selectedTrait->dataId = static_cast<int>(v);
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt = m_selectedTrait->code == TraitCode::Sp_daa_Parameter
                               ? static_cast<int>(m_selectedTrait->value * 100)
                               : 0;
              if (ImGui::SliderInt("##trait_spparameter_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::Sp_daa_Parameter ? "* %d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Attack", nullptr,
                                m_selectedTrait->code >= TraitCode::Attack_Element &&
                                        m_selectedTrait->code <= TraitCode::Attack_Times__plu_ && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::BeginChild("##orpg_traits_attack_left_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            if (ImGui::RadioButton("Attack Element", m_selectedTrait->code == TraitCode::Attack_Element)) {
              if (m_selectedTrait->code != TraitCode::Attack_Element) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Attack_Element;
            }
            if (ImGui::RadioButton("Attack State", m_selectedTrait->code == TraitCode::Attack_State)) {
              if (m_selectedTrait->code != TraitCode::Attack_State) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 1;
              }
              m_selectedTrait->code = TraitCode::Attack_State;
            }
            ImGui::NewLine();
            ImGui::Spacing();
            if (ImGui::RadioButton("Attack Speed", m_selectedTrait->code == TraitCode::Attack_Speed)) {
              if (m_selectedTrait->code != TraitCode::Attack_Speed) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Attack_Speed;
            }
            if (ImGui::RadioButton("Attack Times +", m_selectedTrait->code == TraitCode::Attack_Times__plu_)) {
              if (m_selectedTrait->code != TraitCode::Attack_Times__plu_) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Attack_Times__plu_;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_traits_attack_right_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Attack_Element);
            {
              auto element = dbEditor->element(m_selectedTrait->dataId);
              if (ImGui::BeginCombo("##trait_attack_elem_combo",
                                    m_selectedTrait->code == TraitCode::Attack_Element && element ? element->c_str()
                                                                                                  : "")) {
                for (int i = 1; i < dbEditor->elementsCount(); ++i) {
                  auto elem = dbEditor->element(i);
                  if (ImGui::Selectable(elem ? elem->c_str() : "##traits_editor_empty_attack_elem",
                                        m_selectedTrait->dataId == i)) {
                    m_selectedTrait->dataId = i;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Attack_State);
            {
              if (ImGui::Button(m_selectedTrait->code == TraitCode::Attack_State
                                    ? Database::Instance->stateNameOrId(m_selectedTrait->dataId).c_str()
                                    : "##trait_attack_state_selection",
                                ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_statePicker.emplace("States"sv, Database::Instance->states.states(), m_selectedTrait->dataId);
              }

              int tmpInt =
                  m_selectedTrait->code == TraitCode::Attack_State ? static_cast<int>(m_selectedTrait->value * 100) : 0;
              if (ImGui::SliderInt("##trait_attack_state_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::Attack_State ? "* %d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Attack_Speed);
            {
              float tmp = m_selectedTrait->code == TraitCode::Attack_Speed ? m_selectedTrait->value : 0;
              if (ImGui::SliderFloat("##trait_attack_speed_value", &tmp, -1000, 1000,
                                     m_selectedTrait->code == TraitCode::Attack_Speed ? "%g" : "")) {
                m_selectedTrait->value = tmp;
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Attack_Times__plu_);
            {
              float tmp = m_selectedTrait->code == TraitCode::Attack_Times__plu_ ? m_selectedTrait->value : 0;
              if (ImGui::SliderFloat("##trait_attack_times+_value", &tmp, -9, 9,
                                     m_selectedTrait->code == TraitCode::Attack_Times__plu_ ? "%g" : "")) {
                m_selectedTrait->value = tmp;
              }
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Skill", nullptr,
                                m_selectedTrait->code >= TraitCode::Add_Skill_Type &&
                                        m_selectedTrait->code <= TraitCode::Seal_Skill && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::BeginChild("##orpg_traits_skill_left_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            if (ImGui::RadioButton("Add Skill Type", m_selectedTrait->code == TraitCode::Add_Skill_Type)) {
              if (m_selectedTrait->code != TraitCode::Add_Skill_Type) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Add_Skill_Type;
            }
            if (ImGui::RadioButton("Seal Skill Type", m_selectedTrait->code == TraitCode::Seal_Skill_Type)) {
              if (m_selectedTrait->code != TraitCode::Seal_Skill_Type) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Seal_Skill_Type;
            }
            if (ImGui::RadioButton("Add Skill", m_selectedTrait->code == TraitCode::Add_Skill)) {
              if (m_selectedTrait->code != TraitCode::Add_Skill) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Add_Skill;
            }
            if (ImGui::RadioButton("Seal Skill", m_selectedTrait->code == TraitCode::Seal_Skill)) {
              if (m_selectedTrait->code != TraitCode::Seal_Skill) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Seal_Skill;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_traits_skill_right_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Add_Skill_Type);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Add_Skill_Type
                                       ? Database::Instance->skillTypeNameOrId(m_selectedTrait->dataId)
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_add_skill_type_combo", preview.c_str())) {
                for (int i = 1; i < Database::Instance->system.skillTypes.size(); i++) {
                  if (ImGui::Selectable(Database::Instance->skillTypeNameOrId(i).c_str(),
                                        i == m_selectedTrait->dataId)) {
                    m_selectedTrait->dataId = i;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Seal_Skill_Type);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Seal_Skill_Type
                                       ? Database::Instance->skillTypeNameOrId(m_selectedTrait->dataId)
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_seal_skill_type_combo", preview.c_str())) {
                for (int i = 1; i < Database::Instance->system.skillTypes.size(); i++) {
                  if (ImGui::Selectable(Database::Instance->skillTypeNameOrId(i).c_str(),
                                        i == m_selectedTrait->dataId)) {
                    m_selectedTrait->dataId = i;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Add_Skill);
            {
              if (ImGui::Button(m_selectedTrait->code == TraitCode::Add_Skill
                                    ? Database::Instance->skillNameOrId(m_selectedTrait->dataId).c_str()
                                    : "##trait_add_skill_selection",
                                ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_skillPicker.emplace("Skills"sv, Database::Instance->skills.skills(), m_selectedTrait->dataId);
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Seal_Skill);
            {
              if (ImGui::Button(m_selectedTrait->code == TraitCode::Seal_Skill
                                    ? Database::Instance->skillNameOrId(m_selectedTrait->dataId).c_str()
                                    : "##trait_seal_skill_selection",
                                ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 0})) {
                m_skillPicker.emplace("Skills"sv, Database::Instance->skills.skills(), m_selectedTrait->dataId);
              }
            }
            ImGui::EndDisabled();
          }
          ImGui::EndChild();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Equip", nullptr,
                                m_selectedTrait->code >= TraitCode::Equip_Weapon &&
                                        m_selectedTrait->code <= TraitCode::Slot_Type && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::BeginChild("##orpg_traits_equip_left_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            if (ImGui::RadioButton("Equip Weapon", m_selectedTrait->code == TraitCode::Equip_Weapon)) {
              if (m_selectedTrait->code != TraitCode::Equip_Weapon) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Equip_Weapon;
            }
            if (ImGui::RadioButton("Equip Armor", m_selectedTrait->code == TraitCode::Equip_Armor)) {
              if (m_selectedTrait->code != TraitCode::Equip_Armor) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Equip_Armor;
            }
            if (ImGui::RadioButton("Lock Equip", m_selectedTrait->code == TraitCode::Lock_Equip)) {
              if (m_selectedTrait->code != TraitCode::Lock_Equip) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Lock_Equip;
            }
            if (ImGui::RadioButton("Seal Equip", m_selectedTrait->code == TraitCode::Seal_Equip)) {
              if (m_selectedTrait->code != TraitCode::Seal_Equip) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Seal_Equip;
            }
            if (ImGui::RadioButton("Slot Type", m_selectedTrait->code == TraitCode::Slot_Type)) {
              if (m_selectedTrait->code != TraitCode::Slot_Type) {
                m_selectedTrait->dataId = 0;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Slot_Type;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_traits_equip_right_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Equip_Weapon);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Equip_Weapon
                                       ? Database::Instance->weaponTypeNameOrId(m_selectedTrait->dataId)
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_equip_weapon_type_combo", preview.c_str())) {
                for (int i = 1; i < Database::Instance->system.weaponTypes.size(); i++) {
                  if (ImGui::Selectable(Database::Instance->weaponTypeNameOrId(i).c_str(),
                                        i == m_selectedTrait->dataId)) {
                    m_selectedTrait->dataId = i;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Equip_Armor);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Equip_Armor
                                       ? Database::Instance->armorTypeOrId(m_selectedTrait->dataId)
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_equip_armor_type_combo", preview.c_str())) {
                for (int i = 1; i < Database::Instance->system.armorTypes.size(); i++) {
                  if (ImGui::Selectable(Database::Instance->armorTypeOrId(i).c_str(), i == m_selectedTrait->dataId)) {
                    m_selectedTrait->dataId = i;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Lock_Equip);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Lock_Equip
                                       ? Database::Instance->equipTypeNameOrId(m_selectedTrait->dataId)
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_equip_type_combo", preview.c_str())) {
                for (int i = 1; i < Database::Instance->system.equipTypes.size(); i++) {
                  if (ImGui::Selectable(Database::Instance->equipTypeNameOrId(i).c_str(),
                                        i == m_selectedTrait->dataId)) {
                    m_selectedTrait->dataId = i;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Seal_Equip);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Seal_Equip
                                       ? Database::Instance->equipTypeNameOrId(m_selectedTrait->dataId)
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_seal_equip_type_combo", preview.c_str())) {
                for (int i = 1; i < Database::Instance->system.equipTypes.size(); i++) {
                  if (ImGui::Selectable(Database::Instance->equipTypeNameOrId(i).c_str(),
                                        i == m_selectedTrait->dataId)) {
                    m_selectedTrait->dataId = i;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Slot_Type);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Slot_Type
                                       ? DecodeEnumName(static_cast<SlotType>(m_selectedTrait->dataId))
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_slot_type_combo", preview.c_str())) {
                for (const auto& type : magic_enum::enum_values<SlotType>()) {
                  if (ImGui::Selectable(DecodeEnumName(type).c_str(),
                                        m_selectedTrait->dataId == static_cast<int>(type))) {
                    m_selectedTrait->dataId = static_cast<int>(type);
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
        if (ImGui::BeginTabItem("Other", nullptr,
                                m_selectedTrait->code >= TraitCode::Action_Times__plu_ &&
                                        m_selectedTrait->code <= TraitCode::Party_Ability && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::BeginChild("##orpg_traits_other_left_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            if (ImGui::RadioButton("Action Times +", m_selectedTrait->code == TraitCode::Action_Times__plu_)) {
              if (m_selectedTrait->code != TraitCode::Action_Times__plu_) {
                m_selectedTrait->dataId = 0;
                m_selectedTrait->value = 1;
              }
              m_selectedTrait->code = TraitCode::Action_Times__plu_;
            }
            if (ImGui::RadioButton("Special Flag", m_selectedTrait->code == TraitCode::Special_Flag)) {
              if (m_selectedTrait->code != TraitCode::Special_Flag) {
                m_selectedTrait->dataId = 0;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Special_Flag;
            }
            if (ImGui::RadioButton("Collapse Effect", m_selectedTrait->code == TraitCode::Collapse_Effect)) {
              if (m_selectedTrait->code != TraitCode::Collapse_Effect) {
                m_selectedTrait->dataId = 0;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Collapse_Effect;
            }
            if (ImGui::RadioButton("Party Ability", m_selectedTrait->code == TraitCode::Party_Ability)) {
              if (m_selectedTrait->code != TraitCode::Party_Ability) {
                m_selectedTrait->dataId = 0;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::Party_Ability;
            }
          }
          ImGui::EndChild();
          ImGui::SameLine();
          ImGui::BeginChild("##orpg_traits_other_right_child", ImVec2(0, 0),
                            ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY |
                                ImGuiChildFlags_AlwaysAutoResize,
                            ImGuiWindowFlags_NoBackground);
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Action_Times__plu_);
            {
              int tmpInt = m_selectedTrait->code == TraitCode::Action_Times__plu_
                               ? static_cast<int>(m_selectedTrait->value * 100)
                               : 0;
              if (ImGui::SliderInt("##trait_action_times_plus_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::Action_Times__plu_ ? "%d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.0;
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Special_Flag);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Special_Flag
                                       ? DecodeEnumName(static_cast<SpecialFlag>(m_selectedTrait->dataId))
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_special_flag_combo", preview.c_str())) {
                for (const auto& type : magic_enum::enum_values<SpecialFlag>()) {
                  if (ImGui::Selectable(DecodeEnumName(type).c_str(),
                                        m_selectedTrait->dataId == static_cast<int>(type))) {
                    m_selectedTrait->dataId = static_cast<int>(type);
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Collapse_Effect);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Collapse_Effect
                                       ? DecodeEnumName(static_cast<CollapseEffect>(m_selectedTrait->dataId))
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_collapse_effect_combo", preview.c_str())) {
                for (const auto& type : magic_enum::enum_values<CollapseEffect>()) {
                  if (ImGui::Selectable(DecodeEnumName(type).c_str(),
                                        m_selectedTrait->dataId == static_cast<int>(type))) {
                    m_selectedTrait->dataId = static_cast<int>(type);
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndDisabled();
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Party_Ability);
            {
              const auto preview = m_selectedTrait->code == TraitCode::Party_Ability
                                       ? DecodeEnumName(static_cast<PartyAbility>(m_selectedTrait->dataId))
                                       : "";
              if (ImGui::BeginCombo("##trait_editor_party_ability_combo", preview.c_str())) {
                for (const auto& type : magic_enum::enum_values<PartyAbility>()) {
                  if (ImGui::Selectable(DecodeEnumName(type).c_str(),
                                        m_selectedTrait->dataId == static_cast<int>(type))) {
                    m_selectedTrait->dataId = static_cast<int>(type);
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
        ImGui::EndTabBar();
      }
    }
    ImGui::EndGroup();
    ImGui::Separator();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        if (m_isNewEntry) {
          m_traits->push_back(m_tempTrait);
        }
        m_isNewEntry = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        if (!m_isNewEntry && m_selectedTrait) {
          // Restore values to their unmodified state;
          *m_selectedTrait = m_tempTrait;
        }
        m_isNewEntry = false;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();

    if (m_statePicker) {
      if (auto [closed, confirmed] = m_statePicker->draw(); closed) {
        if (confirmed) {
          m_selectedTrait->dataId = m_statePicker->selection();
        }
        m_statePicker.reset();
      }
    }

    if (m_skillPicker) {
      if (auto [closed, confirmed] = m_skillPicker->draw(); closed) {
        if (confirmed) {
          m_selectedTrait->dataId = m_skillPicker->selection();
        }
        m_skillPicker.reset();
      }
    }
    ImGui::EndPopup();
  }
  m_updateTab = false;
}