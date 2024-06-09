#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor.hpp"

#include "imgui.h"

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
      ImGui::PushID("##traits_editor_trait_dummy");
      ImGui::TableNextRow();
      if (ImGui::TableNextColumn()) {
        ImGui::Selectable("", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);
      }
      ImGui::PopID();
      ImGui::EndTable();
    }
  }
  ImGui::EndGroup();
}