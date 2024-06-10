#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor.hpp"

#include "imgui.h"
#include "imgui_internal.h"

void TraitsEditor::draw(std::vector<Trait>& traits, DatabaseEditor* dbEditor) {
  ImGui::BeginGroup();
  {
    ImGui::SeparatorText("Traits");
    if (ImGui::BeginTable("##orpg_traits_editor", 2,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders |
                              ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollY,
                          ImVec2{ImGui::GetContentRegionMax().x - 15, ImGui::GetContentRegionMax().y - 600})) {
      ImGui::TableSetupColumn("Type");
      ImGui::TableSetupColumn("Content");
      ImGui::TableHeadersRow();

      for (auto& trait : traits) {
        ImGui::PushID(&trait);
        ImGui::TableNextRow();
        if (ImGui::TableNextColumn()) {
          if (ImGui::Selectable(DecodeEnumName(trait.code).c_str(), m_selectedTrait == &trait,
                                ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_selectedTrait = &trait;
              m_isNewEntry = false;
              ImGui::OpenPopup("Traits Edit");
            }
          }
        }
        if (ImGui::TableNextColumn()) {
          switch (trait.code) {
          case TraitCode::Element_Rate:
            ImGui::Text("%s * %i%%", dbEditor->element(trait.dataId).c_str(), static_cast<int>(trait.value) * 100);
            break;

          case TraitCode::Ex_da_Parameter: {
            ImGui::Text("%s + %i%%", DecodeEnumName(static_cast<EXParameterSource>(trait.dataId)).c_str(),
                        static_cast<int>(trait.value) * 100);
            break;
          }
          case TraitCode::Sp_da_Parameter:
            ImGui::Text("%s * %i%%", DecodeEnumName(static_cast<SPParameterSource>(trait.dataId)).c_str(),
                        static_cast<int>(trait.value) * 100);
            break;
          case TraitCode::Parameter:
            ImGui::Text("%s * %i%%", DecodeEnumName(static_cast<ParameterSource>(trait.dataId)).c_str(),
                        static_cast<int>(trait.value) * 100);
            break;
          case TraitCode::Debuff_Rate:
            ImGui::Text("%s * %i%%", DecodeEnumName(static_cast<DebuffSource>(trait.dataId)).c_str(),
                        static_cast<int>(trait.value) * 100);
            break;
          case TraitCode::Attack_State:
          case TraitCode::State_Rate: {
            auto state = dbEditor->state(trait.dataId);
            if (state) {
              ImGui::Text("%s %c %i%%", state->name.c_str(), trait.code == TraitCode::Attack_State ? '+' : '*',
                          static_cast<int>(trait.value) * 100);
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
          case TraitCode::Attack_Element:
            ImGui::Text("%s", dbEditor->element(trait.dataId).c_str());
            break;
          case TraitCode::Attack_Speed:
          case TraitCode::Attack_Times__pl_:
            ImGui::Text("%g", trait.value);
            break;
          case TraitCode::Add_Skill_Type:
          case TraitCode::Seal_Skill_Type: {
            auto skill = dbEditor->skillType(trait.dataId);
            ImGui::Text("%s", skill.c_str());
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
            ImGui::Text("%s", dbEditor->weaponType(trait.dataId).c_str());
            break;
          }
          case TraitCode::Equip_Armor: {
            ImGui::Text("%s", dbEditor->armorType(trait.dataId).c_str());
            break;
          }
          case TraitCode::Lock_Equip:
          case TraitCode::Seal_Equip: {
            ImGui::Text("%s", dbEditor->equipType(trait.dataId).c_str());
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
            ImGui::Text("%i %f", trait.dataId, trait.value);
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
            m_selectedTrait = &m_tempTrait;
            ImGui::OpenPopup("Traits Edit");
          }
        }
      }
      drawPopup(traits, dbEditor);
      ImGui::EndTable();
    }
  }
  ImGui::EndGroup();
}

void TraitsEditor::drawPopup(std::vector<Trait>& traits, DatabaseEditor* dbEditor) {
  if (m_selectedTrait == nullptr) {
    return;
  }

  ImGui::SetNextWindowSize(ImVec2{680, 580} * ImGui::GetIO().FontGlobalScale);
  if (ImGui::BeginPopupModal("Traits Edit", nullptr,
                             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
    if (!m_isNewEntry) {
      // We're not a new entry so copy our values so we can restore them if we cancel
      m_tempTrait = *m_selectedTrait;
    }
    ImGui::BeginGroup();
    {
      if (ImGui::BeginTabBar("##orpg_traits_edit_tab_bar")) {
        if (ImGui::BeginTabItem("Rate")) {
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            {
              if (ImGui::RadioButton("Element Rate", m_selectedTrait->code == TraitCode::Element_Rate)) {
                m_selectedTrait->code = TraitCode::Element_Rate;
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * ImGui::GetIO().FontGlobalScale));
            ImGui::BeginGroup();
            {
              if (m_selectedTrait->code != TraitCode::Element_Rate) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled | ImGuiItemFlags_ReadOnly, true);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
              }
              const auto& element = dbEditor->element(m_selectedTrait->dataId);
              if (ImGui::BeginCombo("##trait_element_rate_combo", element.c_str())) {
                for (int i = 1; i < dbEditor->elementsCount(); ++i) {
                  auto v = dbEditor->element(i);
                  if (v.empty()) {
                    v = "##traits_empty_element_name";
                  }
                  if (ImGui::Selectable(v.c_str(), i == m_selectedTrait->dataId)) {
                    m_selectedTrait->dataId = i;
                  }
                }
                ImGui::EndCombo();
              }
              int tmpInt = m_selectedTrait->code == TraitCode::Element_Rate ? (m_selectedTrait->value * 100) : 0;
              if (ImGui::SliderInt("##trait_element_rate_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::Element_Rate ? "%d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.f;
              }
              if (m_selectedTrait->code != TraitCode::Element_Rate) {
                ImGui::PopItemFlag();
                ImGui::PopStyleColor();
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            if (ImGui::RadioButton("Debuff Rate", m_selectedTrait->code == TraitCode::Debuff_Rate)) {
              m_selectedTrait->code = TraitCode::Debuff_Rate;
            }
            ImGui::EndGroup();
            ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * ImGui::GetIO().FontGlobalScale));
            ImGui::BeginGroup();
            {
              if (m_selectedTrait->code != TraitCode::Debuff_Rate) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled | ImGuiItemFlags_ReadOnly, true);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
              }
              if (ImGui::BeginCombo("##trait_debuff_rate_combo", "")) {
                ImGui::EndCombo();
              }
              int tmpInt = m_selectedTrait->code == TraitCode::Debuff_Rate ? (m_selectedTrait->value * 100) : 0;
              if (ImGui::SliderInt("##trait_debuff_rate_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::Debuff_Rate ? "%d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.f;
              }
              if (m_selectedTrait->code != TraitCode::Debuff_Rate) {
                ImGui::PopItemFlag();
                ImGui::PopStyleColor();
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            if (ImGui::RadioButton("State Rate", m_selectedTrait->code == TraitCode::State_Rate)) {
              m_selectedTrait->code = TraitCode::State_Rate;
            }
            ImGui::EndGroup();
            ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * ImGui::GetIO().FontGlobalScale));
            ImGui::BeginGroup();
            {
              if (m_selectedTrait->code != TraitCode::State_Rate) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled | ImGuiItemFlags_ReadOnly, true);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
              }
              if (ImGui::BeginCombo("##trait_state_rate_combo", "")) {
                ImGui::EndCombo();
              }
              int tmpInt = m_selectedTrait->code == TraitCode::State_Rate ? (m_selectedTrait->value * 100) : 0;
              if (ImGui::SliderInt("##trait_state_rate_value", &tmpInt, 0, 100,
                                   m_selectedTrait->code == TraitCode::State_Rate ? "%d%%" : "")) {
                m_selectedTrait->value = tmpInt / 100.f;
              }
              if (m_selectedTrait->code != TraitCode::State_Rate) {
                ImGui::PopItemFlag();
                ImGui::PopStyleColor();
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            if (ImGui::RadioButton("State Resist", m_selectedTrait->code == TraitCode::State_Resist)) {
              m_selectedTrait->code = TraitCode::State_Resist;
            }
            ImGui::EndGroup();
            ImGui::SameLine((ImGui::GetContentRegionAvail().x / 2) - (104 * ImGui::GetIO().FontGlobalScale));
            ImGui::BeginGroup();
            {
              if (m_selectedTrait->code != TraitCode::State_Resist) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled | ImGuiItemFlags_ReadOnly, true);
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
              }
              if (ImGui::BeginCombo("##trait_state_resist_combo", "")) {
                ImGui::EndCombo();
              }
              if (m_selectedTrait->code != TraitCode::State_Resist) {
                ImGui::PopItemFlag();
                ImGui::PopStyleColor();
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Param")) {
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Attack")) {
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Skill")) {
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Equip")) {
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Other")) {
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
    }
    ImGui::EndGroup();
    if (ImGui::Button("OK")) {
      if (m_isNewEntry) {
        traits.push_back(m_tempTrait);
      }
      m_isNewEntry = false;
      m_selectedTrait = nullptr;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      if (!m_isNewEntry) {
        // Restore values to their unmodified state;
        (*m_selectedTrait) = m_tempTrait;
      }
      m_isNewEntry = false;
      m_selectedTrait = nullptr;
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}