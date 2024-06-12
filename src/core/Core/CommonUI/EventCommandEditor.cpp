#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/Project.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiParsedText.hpp"

#include "imgui.h"
#include "imgui_internal.h"

void EventCommandEditor::draw() {
  ImGui::BeginGroup();
  {
    ImGui::Text("Content:");
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - App::DPIHandler::scale_value(4));
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(16));
    ImGui::PushFont(App::APP->getMonoFont());
    static int item_current_idx = 0; // Here we store our selection data as an index.
    // Custom size: use all width, 5 items tall
    if (ImGui::BeginListBox("##commonevent_code_contents", ImVec2(0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)))) {
      if (ImGui::BeginChild("##orpg_commonevents_editor_commonevent_list", {}, 0, ImGuiWindowFlags_HorizontalScrollbar)) {
        if (m_commands) {
          for (int n = 0; n < m_commands->size(); n++) {
            const bool is_selected = (item_current_idx == n);
            std::string indentPad = m_commands->at(n)->stringRep();

            if (m_commands->at(n)->code() == EventCode::Common_Event) {
              // Common Event + (name)
              CommonEventCommand* cec = dynamic_cast<CommonEventCommand*>(m_commands->at(n).get());
              indentPad += m_project->commentEvent(cec->event)->name.c_str();
            }
            else if (m_commands->at(n)->code() == EventCode::Conditional_Branch) {
              ConditionalBranchCommand* cb = dynamic_cast<ConditionalBranchCommand*>(m_commands->at(n).get());

              if (cb->type == ConditionType::Variable) {
                // Constant and variable sources
                indentPad.replace(indentPad.find("{"), 2, m_project->variable(cb->variable.id).c_str());
                if (cb->variable.source == VariableComparisonSource::Variable) {
                  indentPad.replace(indentPad.find("{"), 2, m_project->variable(cb->variable.otherId).c_str());
                }
              } else if (cb->type == ConditionType::Switch) {
                indentPad.replace(indentPad.find("{"), 2, m_project->switche(cb->globalSwitch.switchIdx).c_str());
              }
              else if (cb->type == ConditionType::Self_Switch) {
                indentPad.replace(indentPad.find("{"), 2, cb->selfSw);
              }
              else if (cb->type == ConditionType::Actor) {
                indentPad.replace(indentPad.find("{"), 2, m_project->actor(cb->actor.id)->name);
                if (cb->actor.type != ActorConditionType::In_The_Party) {
                  ActorConditionType actorCondition = cb->actor.type;
                  if (actorCondition == ActorConditionType::Class) {
                    indentPad.replace(indentPad.find("["), 2, m_project->actorClass(cb->actor.checkId)->name);
                  }
                  else if (actorCondition == ActorConditionType::Skill) {
                    indentPad.replace(indentPad.find("["), 2, m_project->skill(cb->actor.checkId)->name);
                  }
                  else if (actorCondition == ActorConditionType::Weapon) {
                    indentPad.replace(indentPad.find("["), 2, m_project->weapon(cb->actor.checkId)->name);
                  }
                  else if (actorCondition == ActorConditionType::Armor) {
                    indentPad.replace(indentPad.find("["), 2, m_project->armor(cb->actor.checkId)->name);
                  }
                  else if (actorCondition == ActorConditionType::State) {
                    indentPad.replace(indentPad.find("["), 2, m_project->state(cb->actor.checkId)->name);
                  }
                  else {
                    indentPad.replace(indentPad.find("["), 2, cb->name);
                  }
                }
              }
              else if (cb->type == ConditionType::Enemy) {
                indentPad.replace(indentPad.find("{"), 2, m_project->enemy(cb->enemy.id)->name);
                if (cb->enemy.type != EnemyConditionType::Appeared) {
                  indentPad.replace(indentPad.find("["), 2, m_project->state(cb->enemy.stateId)->name);
                }
              }
              else if (cb->type == ConditionType::Character) {
                std::string characterName = "Error!!";
                if (cb->character.id < 0) {
                  characterName = cb->character.id == -1 ? "Player" : "This Event";
                }
                else {
                  characterName = m_project->event(cb->character.id)->name;
                }
                indentPad.replace(indentPad.find("{"), 2, characterName);
              }
              else if (cb->type == ConditionType::Vehicle) {
                indentPad.replace(indentPad.find("{"), 2, m_project->vehicle(cb->vehicle.id));
              }
              else if (cb->type == ConditionType::Gold) {
                indentPad.replace(indentPad.find("{"), 2, std::to_string(cb->gold.value));
              }
              else if (cb->type == ConditionType::Item) {
                indentPad.replace(indentPad.find("{"), 2, m_project->item(cb->item.id)->name);
              }
              else if (cb->type == ConditionType::Weapon) {
                indentPad.replace(indentPad.find("{"), 2, m_project->weapon(cb->item.id)->name);
              }
              else if (cb->type == ConditionType::Armor) {
                indentPad.replace(indentPad.find("{"), 2, m_project->armor(cb->item.id)->name);
              }
              else if (cb->type == ConditionType::Button) {
                indentPad.replace(indentPad.find("{"), 2, cb->button);
              }
              else if (cb->type == ConditionType::Script) {
                indentPad += cb->script;
              }
            }
            else if (m_commands->at(n)->code() == EventCode::Control_Switches) {
              ControlSwitches* cs = dynamic_cast<ControlSwitches*>(m_commands->at(n).get());
              if (cs->start == cs->end) {
                indentPad.replace(indentPad.find("{"), 2, m_project->switche(cs->start));
              }
            }
            else if (m_commands->at(n)->code() == EventCode::Control_Variables) {
              ControlVariables* cv = dynamic_cast<ControlVariables*>(m_commands->at(n).get());

              if (cv->start == cv->end) {
                indentPad.replace(indentPad.find("{"), 2, m_project->variable(cv->start));
                if (cv->operand == VariableControlOperand::Variable) {
                  indentPad.replace(indentPad.find("{"), 2, m_project->variable(cv->variable));
                }
                else if (cv->operand == VariableControlOperand::Game_Data) {
                  if (cv->gameData.source == GameDataSource::Actor) {
                    indentPad.replace(indentPad.find("{"), 2, m_project->actor(cv->gameData.rawSource)->name);
                  }
                  else if (cv->gameData.source == GameDataSource::Armor) {
                    indentPad.replace(indentPad.find("{"), 2, m_project->armor(cv->gameData.rawSource)->name);

                  }
                  else if (cv->gameData.source == GameDataSource::Character) {
                    indentPad.replace(indentPad.find("{"), 2, m_project->event(cv->gameData.rawSource)->name);

                  }
                  else if (cv->gameData.source == GameDataSource::Enemy) {
                    indentPad.replace(indentPad.find("{"), 2, m_project->enemy(cv->gameData.rawSource)->name);

                  }
                  else if (cv->gameData.source == GameDataSource::Item) {
                    indentPad.replace(indentPad.find("{"), 2, m_project->item(cv->gameData.rawSource)->name);

                  }
                  else if (cv->gameData.source == GameDataSource::Weapon) {
                    indentPad.replace(indentPad.find("{"), 2, m_project->weapon(cv->gameData.rawSource)->name);
                  }
                }
              }
            }

            // ImGui::PushStyleColor(ImGuiCol_Text, m_selectedCommonEvent->commands->at(n)->color());
            auto oldCursor = ImGui::GetCursorPos();
            if (ImGui::Selectable("##common_event_selectable", is_selected, 0,
                                  ImVec2{0, ImGui::CalcTextSize(indentPad.c_str()).y}))
              item_current_idx = n;
            ImGui::SetCursorPos(oldCursor);
            /* Nexus: TextParsed takes the syntax of `&<token>[=value];` where value is optional and token is
             * required.
             * Currently there are only 3 tokens, push-color, pop-color, and color, push and pop allow you to
             * control which blocks of text get color, color applies to the whole string
             * if a token is unsupported it won't get processed and will show up in the resulting text
             */
            ImGui::TextParsed("&push-color=255,255,255;%s&pop-color;", indentPad.c_str());
            // ImGui::PopStyleColor();

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndChild();
      }
      ImGui::EndListBox();
    }
    ImGui::PopFont();
  }
  ImGui::EndGroup();
}