#include "Core/DatabaseEditor/DBStatesTab.hpp"
#include "Core/Application.hpp"
#include "Database/States.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "imgui_internal.h"

DBStatesTab::DBStatesTab(States& States, DatabaseEditor* parent) : IDBEditorTab(parent), m_states(States) {
  m_selectedState = m_states.state(1);
  if (m_selectedState) {
    m_traitsEditor.setTraits(&m_selectedState->traits());
  }
}

void DBStatesTab::draw() {
  ImGui::BeginChild("#orpg_states_editor");
  {
    ImGui::BeginChild("##orpg_states_editor_states", ImVec2{250.f, 0} * App::DPIHandler::get_ui_scale(), 0, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("States");
        ImGui::BeginChild("##orpg_states_editor_States_list", ImVec2{0, ImGui::GetContentRegionMax().y - (App::DPIHandler::scale_value(108))});
        {
          ImGui::BeginGroup();
          {
            for (auto& state : m_states.states()) {
              if (state.id() == 0) {
                continue;
              }

              if (ImGui::Selectable(Database::instance()->stateNameAndId(state.id()).c_str(), &state == m_selectedState) || (ImGui::IsItemFocused() && m_selectedState != &state)) {
                m_selectedState = &state;
                m_traitsEditor.setTraits(&m_selectedState->traits());
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max States %i", m_states.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - (App::DPIHandler::scale_value(8)), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxStates = m_states.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_states_editor_states_state_properties");
    {
      if (m_selectedState) {
        ImGui::BeginChild("##orpg_states_state_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedState->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_states_editor_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2)) {
                m_selectedState->setName(name);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            // Image
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Icon:");
              ImGui::Text("%s", std::to_string(m_selectedState->iconIndex()).c_str());
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            const auto* iconSheet = m_parent->getIconSheet();
            auto [min, max] = iconSheet->rectForId(m_selectedState->iconIndex());
            ImGui::Image(iconSheet->texture(), ImVec2{static_cast<float>(iconSheet->iconWidth()), static_cast<float>(iconSheet->iconHeight())}, min,
                         max); // Show icon image

            ImGui::BeginGroup();
            {
              ImGui::Text("Restriction:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::BeginCombo("##database_state_restriction_combo", DecodeEnumName(m_selectedState->restriction()).c_str())) {
                for (const auto& type : magic_enum::enum_values<StateRestriction>()) {
                  if (ImGui::Selectable(DecodeEnumName(type).c_str(), m_selectedState->restriction() == type)) {
                    m_selectedState->setRestriction(type);
                  }
                }
                ImGui::EndCombo();
              }
              ImGui::EndGroup();
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Priority:");

              int tempInt = m_selectedState->priority();
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::InputInt("##database_state_priority", &tempInt)) {
                m_selectedState->setPriority(std::clamp(tempInt, 0, 100));
              }
              ImGui::EndGroup();
            }
            ImGui::BeginGroup();
            {
              ImGui::Text("[SV] Motion:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::BeginCombo("##database_state_sv_motion_combo", DecodeEnumName(m_selectedState->motion()).c_str())) {
                for (const auto& type : magic_enum::enum_values<StateMotion>()) {
                  if (ImGui::Selectable(DecodeEnumName(type).c_str(), m_selectedState->motion() == type)) {
                    m_selectedState->setMotion(type);
                  }
                }
                ImGui::EndCombo();
              }
              ImGui::EndGroup();
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("[SV] Overlay:");
              ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
              if (ImGui::BeginCombo("##database_state_sv_overlay_combo", DecodeEnumName(m_selectedState->overlay()).c_str())) {
                for (const auto& type : magic_enum::enum_values<StateOverlay>()) {
                  if (ImGui::Selectable(DecodeEnumName(type).c_str(), m_selectedState->overlay() == type)) {
                    m_selectedState->setOverlay(type);
                  }
                }
                ImGui::EndCombo();
              }
              ImGui::EndGroup();
            }
          }
          ImGui::EndGroup();

          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Removal Conditions");
            ImGui::BeginGroup();
            {
              bool isRemoveAtBattleEnd = m_selectedState->removeAtBattleEnd();
              if (ImGui::Checkbox("##database_states_remove_battleend_checkbox", &isRemoveAtBattleEnd)) {
                m_selectedState->setRemoveAtBattleEnd(isRemoveAtBattleEnd);
              }
              ImGui::SameLine();
              ImGui::Text("Remove at Battle End");
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
            {
              bool isRemoveByRestriction = m_selectedState->removeByRestriction();
              if (ImGui::Checkbox("##database_states_remove_removerestr_checkbox", &isRemoveByRestriction)) {
                m_selectedState->setRemoveByRestriction(isRemoveByRestriction);
              }
              ImGui::SameLine();
              ImGui::Text("Remove by Restriction");
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              // Auto-removing timing
              ImGui::BeginGroup();
              {
                ImGui::Text("Auto-removal Timing:");
                ImGui::SameLine();
                ImGui::EndGroup();
              }
              ImGui::SameLine();
              ImGui::BeginGroup();
              {
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(170));
                if (ImGui::BeginCombo("##database_state_removal_timing_combo", DecodeEnumName(m_selectedState->autoRemovalTiming()).c_str())) {
                  for (const auto& type : magic_enum::enum_values<AutoRemovalTiming>()) {
                    if (ImGui::Selectable(DecodeEnumName(type).c_str(), m_selectedState->autoRemovalTiming() == type)) {
                      m_selectedState->setAutoRemovalTiming(type);
                    }
                  }
                  ImGui::EndCombo();
                }
                ImGui::EndGroup();
              }

              ImGui::EndGroup();
            }
            ImGui::BeginGroup();
            {
              ImGui::BeginDisabled(m_selectedState->autoRemovalTiming() == AutoRemovalTiming::None);
              {
                // Duration in turns
                ImGui::Text("Duration in Turns:");
                int tempInt = m_selectedState->minTurns();
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(130));
                if (ImGui::InputInt("##database_state_duration_turns_1", &tempInt)) {
                  m_selectedState->setMinTurns(std::clamp(tempInt, 1, 9999));
                }
                ImGui::SameLine();
                ImGui::Text("~");
                ImGui::SameLine();
                int tempInt2 = m_selectedState->maxTurns();
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(130));
                if (ImGui::InputInt("##database_state_duration_turns_2", &tempInt2)) {
                  m_selectedState->setMaxTurns(std::clamp(tempInt2, 1, 9999));
                }
              }
              ImGui::EndDisabled();
              ImGui::EndGroup();
            }
            ImGui::BeginGroup();
            {
              // Remove by Damage
              bool isRemoveByDamage = m_selectedState->removeByDamage();
              if (ImGui::Checkbox("##database_states_removedamage_checkbox", &isRemoveByDamage)) {
                m_selectedState->setRemoveByDamage(isRemoveByDamage);
              }
              ImGui::SameLine();
              ImGui::Text("Remove by Damage");
              ImGui::SameLine();
              ImGui::BeginDisabled(!isRemoveByDamage);
              {
                int tempInt = m_selectedState->chanceByDamage();
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
                if (ImGui::SliderInt("##database_states_removebydamage_chance", &tempInt, 0, 100, isRemoveByDamage ? " %d%%" : "")) {
                  m_selectedState->setChanceByDamage(tempInt);
                }
                ImGui::EndDisabled();
              }
              ImGui::EndGroup();
            }
            ImGui::BeginGroup();
            {
              // Remove by Walking
              bool isRemoveByWalking = m_selectedState->stepsToRemove();
              if (ImGui::Checkbox("##database_states_removebywalking_checkbox", &isRemoveByWalking)) {
                m_selectedState->setRemoveByWalking(isRemoveByWalking);
              }
              ImGui::SameLine();
              ImGui::Text("Remove by Walking");
              ImGui::SameLine();
              ImGui::BeginDisabled(!isRemoveByWalking);
              {
                int tempInt = m_selectedState->stepsToRemove();
                ImGui::SetNextItemWidth(App::DPIHandler::scale_value(150));
                if (ImGui::SliderInt("##database_states_stepstoremove", &tempInt, 0, 100, isRemoveByWalking ? " %d%%" : "")) {
                  m_selectedState->setStepsToRemove(tempInt);
                }
                ImGui::SameLine();
                ImGui::Text("(steps)");
                ImGui::EndDisabled();
              }
              ImGui::EndGroup();
            }
            ImGui::EndGroup();
          }
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Messages");
            ImGui::EndGroup();
          }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_states_state_panel_right");
        {
          m_traitsEditor.draw(m_parent);
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedState->note().c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_database_states_note", note, IM_ARRAYSIZE(note),
                                          ImVec2{ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(16), ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {
              m_selectedState->setNote(note);
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
      }

      ImGui::EndChild();
    }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max States", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the state list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxStates);
        ImGui::SameLine();
        if (ImGui::Button("Accept")) {
          m_changeConfirmDialogOpen = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_changeIntDialogOpen = false;
        }
      }
      ImGui::End();

      if (m_changeConfirmDialogOpen) {
        if (ImGui::Begin("Confirm Change", &m_changeConfirmDialogOpen,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
          ImGui::Text("Are you sure?");
          if (ImGui::Button("Yes")) {
            const int tmpId = m_selectedState->id();
            m_states.resize(m_editMaxStates);
            m_selectedState = m_states.state(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_editMaxStates = m_states.count();
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
  }
}