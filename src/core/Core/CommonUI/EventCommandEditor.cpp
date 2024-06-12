#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/Project.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiParsedText.hpp"

#include "imgui.h"
#include "imgui_internal.h"

void insertValue(std::string& indentPad, const std::string& val, const std::string& delim) {
  auto pos = indentPad.find(delim);
  if (pos != std::string::npos) {
    indentPad.replace(pos, 2, val);
  }
}
void EventCommandEditor::draw() {
  ImGui::BeginGroup();
  {
    ImGui::Text("Content:");
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - App::DPIHandler::scale_value(4));
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - App::DPIHandler::scale_value(16));
    ImGui::PushFont(App::APP->getMonoFont());
    static int item_current_idx = 0; // Here we store our selection data as an index.
    // Custom size: use all width, 5 items tall
    if (ImGui::BeginListBox("##commonevent_code_contents",
                            ImVec2(0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)))) {
      ImGui::BeginChild("##orpg_commonevents_editor_commonevent_list", {}, 0, ImGuiWindowFlags_HorizontalScrollbar);
      {
        if (m_commands) {
          for (int n = 0; n < m_commands->size(); n++) {
            const bool is_selected = (item_current_idx == n);
            std::string indentPad = m_commands->at(n)->stringRep();

            if (m_commands->at(n)->code() == EventCode::Common_Event) {
              // Common Event + (name)
              CommonEventCommand* cec = dynamic_cast<CommonEventCommand*>(m_commands->at(n).get());
              indentPad += m_project->commonEvent(cec->event)->name.c_str();
            } else if (m_commands->at(n)->code() == EventCode::Conditional_Branch) {
              ConditionalBranchCommand* cb = dynamic_cast<ConditionalBranchCommand*>(m_commands->at(n).get());

              if (cb->type == ConditionType::Variable) {
                // Constant and variable sources
                insertValue(indentPad, m_project->variable(cb->variable.id).c_str(), "{");
                if (cb->variable.source == VariableComparisonSource::Variable) {
                  insertValue(indentPad, m_project->variable(cb->variable.otherId).c_str(), "{");
                }
              } else if (cb->type == ConditionType::Switch) {
                insertValue(indentPad, m_project->switche(cb->globalSwitch.switchIdx).c_str(), "{");
              } else if (cb->type == ConditionType::Self_Switch) {
                insertValue(indentPad, cb->selfSw, "{");
              } else if (cb->type == ConditionType::Actor) {
                insertValue(indentPad, m_project->actor(cb->actor.id)->name, "{");
                if (cb->actor.type != ActorConditionType::In_The_Party) {
                  ActorConditionType actorCondition = cb->actor.type;
                  if (actorCondition == ActorConditionType::Class) {
                    insertValue(indentPad, m_project->actorClass(cb->actor.checkId)->name, "[");
                  } else if (actorCondition == ActorConditionType::Skill) {
                    insertValue(indentPad, m_project->skill(cb->actor.checkId)->name, "[");
                  } else if (actorCondition == ActorConditionType::Weapon) {
                    insertValue(indentPad, m_project->weapon(cb->actor.checkId)->name, "[");
                  } else if (actorCondition == ActorConditionType::Armor) {
                    insertValue(indentPad, m_project->armor(cb->actor.checkId)->name, "{");
                  } else if (actorCondition == ActorConditionType::State) {
                    insertValue(indentPad, m_project->state(cb->actor.checkId)->name, "{");
                  } else {
                    insertValue(indentPad, cb->name, "{");
                  }
                }
              } else if (cb->type == ConditionType::Enemy) {
                insertValue(indentPad, m_project->enemy(cb->enemy.id)->name, "{");
                if (cb->enemy.type != EnemyConditionType::Appeared) {
                  insertValue(indentPad, m_project->state(cb->enemy.stateId)->name, "{");
                }
              } else if (cb->type == ConditionType::Character) {
                std::string characterName = "Error!!";
                if (cb->character.id < 0) {
                  characterName = cb->character.id == -1 ? "Player" : "This Event";
                } else {
                  characterName = m_project->event(cb->character.id)->name;
                }
                insertValue(indentPad, characterName, "{");
              } else if (cb->type == ConditionType::Vehicle) {
                insertValue(indentPad, m_project->vehicle(cb->vehicle.id), "{");
              } else if (cb->type == ConditionType::Gold) {
                insertValue(indentPad, std::to_string(cb->gold.value), "{");
              } else if (cb->type == ConditionType::Item) {
                insertValue(indentPad, m_project->item(cb->item.id)->name, "{");
              } else if (cb->type == ConditionType::Weapon) {
                insertValue(indentPad, m_project->weapon(cb->item.id)->name, "{");
              } else if (cb->type == ConditionType::Armor) {
                insertValue(indentPad, m_project->armor(cb->item.id)->name, "{");
              } else if (cb->type == ConditionType::Button) {
                insertValue(indentPad, cb->button, "{");
              } else if (cb->type == ConditionType::Script) {
                indentPad += cb->script;
              }
            } else if (m_commands->at(n)->code() == EventCode::Control_Switches) {
              ControlSwitches* cs = dynamic_cast<ControlSwitches*>(m_commands->at(n).get());
              if (cs->start == cs->end) {
                insertValue(indentPad, m_project->switche(cs->start), "{");
              }
            } else if (m_commands->at(n)->code() == EventCode::Control_Variables) {
              ControlVariables* cv = dynamic_cast<ControlVariables*>(m_commands->at(n).get());

              if (cv->start == cv->end) {
                insertValue(indentPad, m_project->variable(cv->start), "{");
                if (cv->operand == VariableControlOperand::Variable) {
                  insertValue(indentPad, m_project->variable(cv->variable), "{");
                } else if (cv->operand == VariableControlOperand::Game_Data) {
                  if (cv->gameData.source == GameDataSource::Actor) {
                    insertValue(indentPad, m_project->actor(cv->gameData.rawSource)->name, "{");
                  } else if (cv->gameData.source == GameDataSource::Armor) {
                    insertValue(indentPad, m_project->armor(cv->gameData.rawSource)->name, "{");

                  } else if (cv->gameData.source == GameDataSource::Character) {
                    insertValue(indentPad, m_project->event(cv->gameData.rawSource)->name, "{");

                  } else if (cv->gameData.source == GameDataSource::Enemy) {
                    insertValue(indentPad, m_project->enemy(cv->gameData.rawSource)->name, "{");

                  } else if (cv->gameData.source == GameDataSource::Item) {
                    insertValue(indentPad, m_project->item(cv->gameData.rawSource)->name, "{");

                  } else if (cv->gameData.source == GameDataSource::Weapon) {
                    insertValue(indentPad, m_project->weapon(cv->gameData.rawSource)->name, "{");
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
      }
      ImGui::EndChild();
      ImGui::EndListBox();
    }
    ImGui::PopFont();
  }
  ImGui::EndGroup();
}