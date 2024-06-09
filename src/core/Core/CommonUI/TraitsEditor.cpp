#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor.hpp"

#include "imgui.h"

void TraitsEditor(std::vector<Trait>& traits, DatabaseEditor* dbEditor) {
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
          char str[4096];
          snprintf(str, 4096, "%s", DecodeEnumName(trait.code).c_str());
          ImGui::Selectable(str, false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap);
        }
        if (ImGui::TableNextColumn()) {
          switch (trait.code) {
          case TraitCode::Element_Rate:
            ImGui::Text("%s * %i%%", dbEditor->element(trait.dataId).c_str(), static_cast<int>(trait.value) * 100);
            break;
          case TraitCode::Debuff_Rate:
            ImGui::Text("%s * %i%%", DecodeEnumName(static_cast<DebuffRateSource>(trait.dataId)).c_str(),
                        static_cast<int>(trait.value) * 100);
            break;
          case TraitCode::State_Rate: {
            auto state = dbEditor->state(trait.dataId);
            if (state) {
              ImGui::Text("%s * %i%%", state->name.c_str(), static_cast<int>(trait.value) * 100);
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