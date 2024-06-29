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
            auto elem = dbEditor->element(trait.dataId);
            if (elem) {
              ImGui::Text("%s * %i%%", elem->c_str(), static_cast<int>(trait.value * 100));
            }
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
            if (const auto state = dbEditor->state(trait.dataId)) {
              ImGui::Text("%s %c %i%%", state->name.c_str(), trait.code == TraitCode::Attack_State ? '+' : '*',
                          static_cast<int>(trait.value * 100));
            } else {
              ImGui::Text("Data Error! state %i not found", trait.dataId);
            }
            break;
          }
          case TraitCode::State_Resist: {
            auto state = dbEditor->state(trait.dataId);
            if (state) {
              ImGui::Text("%s", state->name.c_str());
            } else {
              ImGui::Text("Data Error! state %i not found", trait.dataId);
            }
            break;
          }
          case TraitCode::Attack_Element: {
            auto elem = dbEditor->element(trait.dataId);
            if (elem) {
              ImGui::Text("%s", elem->c_str());
            }
            break;
          }
          case TraitCode::Attack_Speed:
          case TraitCode::Attack_Times__pl_:
            ImGui::Text("%g", trait.value);
            break;
          case TraitCode::Add_Skill_Type:
          case TraitCode::Seal_Skill_Type: {
            auto skill = dbEditor->skillType(trait.dataId);
            if (skill) {
              ImGui::Text("%s", skill->c_str());
            }
            break;
          }
          case TraitCode::Add_Skill:
          case TraitCode::Seal_Skill: {
            auto skill = dbEditor->skill(trait.dataId);
            if (skill) {
              ImGui::Text("%s", skill->name.c_str());
            } else {
              ImGui::Text("Data Error! skill %i not found", trait.dataId);
            }
            break;
          }
          case TraitCode::Equip_Weapon: {
            auto weapon = dbEditor->weaponType(trait.dataId);
            if (weapon) {
              ImGui::Text("%s", weapon->c_str());
            }
            break;
          }
          case TraitCode::Equip_Armor: {
            auto armorType = dbEditor->armorType(trait.dataId);
            if (armorType) {
              ImGui::Text("%s", armorType->c_str());
            }
            break;
          }
          case TraitCode::Lock_Equip:
          case TraitCode::Seal_Equip: {
            auto equip = dbEditor->equipType(trait.dataId);
            if (equip) {
              ImGui::Text("%s", equip->c_str());
            }
            break;
          }
          case TraitCode::Slot_Type:
            ImGui::Text("%s", DecodeEnumName(static_cast<SlotType>(trait.dataId)).c_str());
            break;
          case TraitCode::Action_Times__pl_:
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

  ImGui::SetNextWindowSize(ImVec2{680, 550} * App::DPIHandler::get_ui_scale());
  if (ImGui::BeginPopupModal(TraitsEditorPopupId.data(), nullptr,
                             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
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
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            {
              if (ImGui::RadioButton("Element Rate", m_selectedTrait->code == TraitCode::Element_Rate)) {
                if (m_selectedTrait->code != TraitCode::State_Resist) {
                  m_selectedTrait->dataId = 1;
                  m_selectedTrait->value = 1;
                }
                m_selectedTrait->code = TraitCode::Element_Rate;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * App::DPIHandler::get_ui_scale()));
            ImGui::BeginGroup();
            {
              ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Element_Rate);
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
              ImGui::EndDisabled();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            if (ImGui::RadioButton("Debuff Rate", m_selectedTrait->code == TraitCode::Debuff_Rate)) {
              if (m_selectedTrait->code != TraitCode::Debuff_Rate) {
                m_selectedTrait->dataId = static_cast<int>(DebuffSource::Max_HP);
                m_selectedTrait->value = 1;
              }
              m_selectedTrait->code = TraitCode::Debuff_Rate;
            }
            ImGui::EndGroup();
            ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * App::DPIHandler::get_ui_scale()));
            ImGui::BeginGroup();
            {
              ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Debuff_Rate);
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
              ImGui::EndDisabled();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            if (ImGui::RadioButton("State Rate", m_selectedTrait->code == TraitCode::State_Rate)) {
              if (m_selectedTrait->code != TraitCode::State_Rate) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 1;
              }
              m_selectedTrait->code = TraitCode::State_Rate;
            }
            ImGui::EndGroup();
            ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * App::DPIHandler::get_ui_scale()));
            ImGui::BeginGroup();
            {
              ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::State_Rate);
              const auto state = dbEditor->state(m_selectedTrait->dataId);
              const std::string preview = state && m_selectedTrait->code == TraitCode::State_Rate
                                              ? std::format("{:04} {}", state->id, state->name)
                                              : "";
              if (ImGui::BeginCombo("##trait_state_rate_combo", preview.c_str())) {
                for (const auto& state : dbEditor->states().states()) {
                  if (!state.m_isValid) {
                    continue;
                  }

                  if (ImGui::Selectable(std::format("{:04} {}", state.id, state.name).c_str(),
                                        m_selectedTrait->dataId == state.id)) {
                    m_selectedTrait->dataId = state.id;
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt =
                  m_selectedTrait->code == TraitCode::State_Rate ? static_cast<int>(m_selectedTrait->value * 100) : 0;
              if (ImGui::SliderInt("##trait_state_rate_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::State_Rate ? "* %d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.0;
              }
              ImGui::EndDisabled();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            if (ImGui::RadioButton("State Resist", m_selectedTrait->code == TraitCode::State_Resist)) {
              if (m_selectedTrait->code != TraitCode::State_Resist) {
                m_selectedTrait->dataId = 1;
                m_selectedTrait->value = 0;
              }
              m_selectedTrait->code = TraitCode::State_Resist;
            }
            ImGui::EndGroup();
            ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * App::DPIHandler::get_ui_scale()));
            ImGui::BeginGroup();
            {
              ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::State_Resist);
              const auto state = dbEditor->state(m_selectedTrait->dataId);
              const std::string preview = state && m_selectedTrait->code == TraitCode::State_Resist
                                              ? std::format("{:04} {}", state->id, state->name)
                                              : "";
              if (ImGui::BeginCombo("##trait_state_resist_combo", preview.c_str())) {
                for (const auto& state : dbEditor->states().states()) {
                  if (!state.m_isValid) {
                    continue;
                  }

                  if (ImGui::Selectable(std::format("{:04} {}", state.id, state.name).c_str(),
                                        m_selectedTrait->dataId == state.id)) {
                    m_selectedTrait->dataId = state.id;
                  }
                }
                ImGui::EndCombo();
              }
              ImGui::EndDisabled();
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Param", nullptr,
                                m_selectedTrait->code >= TraitCode::Parameter &&
                                        m_selectedTrait->code <= TraitCode::Sp_daa_Parameter && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::BeginGroup();
          if (ImGui::RadioButton("Parameter", m_selectedTrait->code == TraitCode::Parameter)) {
            if (m_selectedTrait->code != TraitCode::Parameter) {
              m_selectedTrait->dataId = static_cast<int>(ParameterSource::Max_HP);
              m_selectedTrait->value = 1;
            }
            m_selectedTrait->code = TraitCode::Parameter;
          }
          ImGui::EndGroup();
          ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * App::DPIHandler::get_ui_scale()));
          ImGui::BeginGroup();
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Parameter);
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
            ImGui::EndDisabled();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          if (ImGui::RadioButton("Ex-Parameter", m_selectedTrait->code == TraitCode::Ex_daa_Parameter)) {
            if (m_selectedTrait->code != TraitCode::Ex_daa_Parameter) {
              m_selectedTrait->dataId = static_cast<int>(EXParameterSource::Hit_Rate);
              m_selectedTrait->value = 0;
            }
            m_selectedTrait->code = TraitCode::Ex_daa_Parameter;
          }
          ImGui::EndGroup();
          ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * App::DPIHandler::get_ui_scale()));
          ImGui::BeginGroup();
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Ex_daa_Parameter);
            if (ImGui::BeginCombo("##trait_exparameter_combo",
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
            ImGui::EndDisabled();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          if (ImGui::RadioButton("Sp-Parameter", m_selectedTrait->code == TraitCode::Sp_daa_Parameter)) {
            if (m_selectedTrait->code != TraitCode::Sp_daa_Parameter) {
              m_selectedTrait->dataId = static_cast<int>(SPParameterSource::Target_Rate);
              m_selectedTrait->value = 1;
            }
            m_selectedTrait->code = TraitCode::Sp_daa_Parameter;
          }
          ImGui::EndGroup();
          ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * App::DPIHandler::get_ui_scale()));
          ImGui::BeginGroup();
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Sp_daa_Parameter);
            if (ImGui::BeginCombo("##trait_spparameter_combo",
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
            ImGui::EndDisabled();
          }
          ImGui::EndGroup();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Attack", nullptr,
                                m_selectedTrait->code >= TraitCode::Attack_Element &&
                                        m_selectedTrait->code <= TraitCode::Attack_Times__pl_ && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::BeginGroup();
          if (ImGui::RadioButton("Attack Element", m_selectedTrait->code == TraitCode::Attack_Element)) {
            if (m_selectedTrait->code != TraitCode::Attack_Element) {
              m_selectedTrait->dataId = 1;
              m_selectedTrait->value = 0;
            }
            m_selectedTrait->code = TraitCode::Attack_Element;
          }
          ImGui::EndGroup();
          ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * App::DPIHandler::get_ui_scale()));
          ImGui::BeginGroup();
          {
            ImGui::BeginDisabled(m_selectedTrait->code != TraitCode::Attack_Element);
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
            ImGui::EndDisabled();
          }
          ImGui::EndGroup();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Skill", nullptr,
                                m_selectedTrait->code >= TraitCode::Add_Skill_Type &&
                                        m_selectedTrait->code <= TraitCode::Seal_Skill && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Equip", nullptr,
                                m_selectedTrait->code >= TraitCode::Equip_Weapon &&
                                        m_selectedTrait->code <= TraitCode::Slot_Type && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Other", nullptr,
                                m_selectedTrait->code >= TraitCode::Action_Times__pl_ &&
                                        m_selectedTrait->code <= TraitCode::Party_Ability && m_updateTab
                                    ? ImGuiTabItemFlags_SetSelected
                                    : 0)) {
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
    ImGui::EndPopup();
  }
  m_updateTab = false;
}