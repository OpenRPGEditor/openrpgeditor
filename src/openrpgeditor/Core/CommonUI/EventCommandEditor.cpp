#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/ImGuiExt/ImGuiParsedText.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"

#include "Core/Log.hpp"
#include "Database/EventCommands/RepeatAbove.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include <clip.h>

#include <iostream>
#include <vector>

static clip::format RPGMVEventCommandFormat = -1;

void insertValue(std::string& indentPad, const std::string& val, const std::string& delim) {
  auto pos = indentPad.find(delim);
  if (pos != std::string::npos) {
    indentPad.replace(pos, 2, val);
  }
}
static bool isNestable(const std::shared_ptr<IEventCommand>& selectedCmd) {
  return selectedCmd->code() == EventCode::Conditional_Branch || selectedCmd->code() == EventCode::Else || selectedCmd->code() == EventCode::Loop || selectedCmd->code() == EventCode::If_Win ||
         selectedCmd->code() == EventCode::If_Lose || selectedCmd->code() == EventCode::If_Escape || selectedCmd->code() == EventCode::Show_Choices ||
         selectedCmd->code() == EventCode::When_Selected || selectedCmd->code() == EventCode::When_Cancel;
}

static bool isNestableEnd(const std::shared_ptr<IEventCommand>& selectedCmd) {
  return selectedCmd->code() == EventCode::Repeat_Above || selectedCmd->code() == EventCode::End || selectedCmd->code() == EventCode::End_del_Battle_Processing ||
         selectedCmd->code() == EventCode::End_del_ShowChoices || selectedCmd->code() == EventCode::Else || selectedCmd->code() == EventCode::If_Lose || selectedCmd->code() == EventCode::If_Escape ||
         selectedCmd->code() == EventCode::When_Selected || selectedCmd->code() == EventCode::When_Cancel;
}
void EventCommandEditor::blockSelect(const int n) {
  if (!m_hasFocus) {
    return;
  }

  if (m_commands->at(n)->hasPartner()) {
    if (!m_commands->at(n)->reverseSelection()) {
      int j = n + 1;
      int partnerCount = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl) || m_commands->at(n)->isCollapsed() ? m_commands->at(n)->partnerCount() : 1;
      while (partnerCount) {
        if (j >= m_commands->size() || *m_commands->at(j)->indent < *m_commands->at(n)->indent) {
          break;
        }

        while (true) {
          if (j >= m_commands->size() || m_commands->at(n)->isPartner(m_commands->at(j)->code(), m_commands->at(j)->indent)) {
            partnerCount--;
            if (partnerCount > 0) {
              ++j;
            }
            break;
          }
          ++j;
        }
        if (j >= m_commands->size() - 1 || (m_commands->at(j) && m_commands->at(j)->code() == EventCode::Event_Dummy)) {
          j--;
        }
      }
      m_selectedEnd = j;
    } else {
      m_selectedEnd = n;
      int j = n - 1;
      int partnerCount = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl) ? m_commands->at(n)->partnerCount() : 1;
      while (partnerCount--) {
        if (j < 0 || *m_commands->at(j)->indent < *m_commands->at(n)->indent) {
          if (*m_commands->at(j)->indent < *m_commands->at(n)->indent) {
            ++j;
          }
          break;
        }
        while (true) {
          if (j < 0 || m_commands->at(n)->isPartner(m_commands->at(j)->code(), m_commands->at(j)->indent)) {
            break;
          }
          --j;
        }
      }
      m_selectedCommand = j;
    }
  } else {
    m_selectedEnd = -1;
  }
}

void EventCommandEditor::handleClipboardInteraction() const {
  if (!m_hasFocus) {
    return;
  }

  if (RPGMVEventCommandFormat == -1) {
    RPGMVEventCommandFormat = clip::register_format("application/rpgmv-EventCommand");
  }
  if (ImGui::IsKeyPressed(ImGuiKey_V) && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))) {
    clip::lock l;
    if (l.is_convertible(RPGMVEventCommandFormat)) {
      auto len = l.get_data_length(RPGMVEventCommandFormat);
      std::string cmd;
      cmd.resize(len);
      if (l.get_data(RPGMVEventCommandFormat, cmd.data(), len)) {
        nlohmann::ordered_json cmdJson = nlohmann::ordered_json::parse(cmd);
        CommandParser parser;
        auto commands = parser.parse(cmdJson);
        int curIndent = m_selectedCommand > 0 ? *m_commands->at(m_selectedCommand)->indent : 0;
        int nestedCount = 0;
        if (isNestable(commands[0])) {
          ++nestedCount;
        }

        for (auto& command : commands) {
          if (!command->indent) {
            command->indent.emplace();
          }
          if (isNestableEnd(command)) {
            if (nestedCount > 0 && curIndent > 0) {
              curIndent--;
            }
            nestedCount--;
          }
          *command->indent = curIndent;
          if (isNestable(command)) {
            curIndent++;
            nestedCount++;
          }
        }
        m_commands->insert(m_commands->begin() + m_selectedCommand, commands.begin(), commands.end());
      }
    }
  } else if (ImGui::IsKeyPressed(ImGuiKey_C) && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))) {
    clip::lock l;
    int start = m_selectedCommand;
    int end = m_selectedEnd == -1 ? m_selectedCommand + 1 : m_selectedEnd + 1;
    std::vector<std::shared_ptr<IEventCommand>> commands(m_commands->begin() + start, m_commands->begin() + end);
    nlohmann::ordered_json cmdJson;
    CommandParser::serialize(cmdJson, commands);
    auto v = cmdJson.dump();
    l.set_data(RPGMVEventCommandFormat, v.data(), v.size());
  } else if (ImGui::IsKeyPressed(ImGuiKey_X) && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))) {
    if (m_commands->at(m_selectedCommand)->code() != EventCode::Event_Dummy) {
      clip::lock l;
      int start = m_selectedCommand;
      int end = m_selectedEnd == -1 ? m_selectedCommand + 1 : m_selectedEnd + 1;
      std::vector<std::shared_ptr<IEventCommand>> commands(m_commands->begin() + start, m_commands->begin() + end);
      nlohmann::ordered_json cmdJson;
      CommandParser::serialize(cmdJson, commands);
      auto v = cmdJson.dump();
      l.set_data(RPGMVEventCommandFormat, v.data(), v.size());
      m_commands->erase(m_commands->begin() + start, m_commands->begin() + end);
    }
  }
}
void EventCommandEditor::setupTableHeader() {
  ImGui::TableSetupColumn("Step##command_selectable_column", ImGuiTableFlags_SizingFixedFit);
  ImGui::TableSetupColumn("##command_collapse_column", ImGuiTableFlags_SizingFixedFit);
  ImGui::TableSetupColumn("Command Operation");
  ImGui::TableSetupScrollFreeze(3, 1);
  ImGui::TableHeadersRow();
}
void EventCommandEditor::setupTableColors() {
  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.37f, 0.37f, 0.37f, 0.43f));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.37f, 0.37f, 0.37f, 0.78f));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.37f, 0.37f, 0.37f, 0.68f));
  ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(0.03f, 0.21f, 0.26f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(0.04f, 0.16f, 0.19f, 1.00f));
}
void EventCommandEditor::handleBlockCollapse(int& n) const {
  if (m_commands->at(n)->collapsable()) {
    int oldN = n;
    auto cmd = m_commands->at(n);
    std::string tooltip;
    if (cmd->isCollapsed()) {
      auto next = m_commands->at(n + 1);
      bool addedElipses = false;
      while (!cmd->isPartner(next->code(), next->indent)) {
        if ((n + 1) >= m_commands->size() - 1) {
          break;
        }
        n++;

        auto rep = next->stringRep(*Database::instance());
        if (next->code() != EventCode::Event_Dummy) {
          if (std::ranges::count(tooltip.begin(), tooltip.end(), '\n') < 5) {
            tooltip += rep + "\n";
          } else if (!addedElipses) {
            tooltip += next->indentText(*next->indent) + "\u2026";
            addedElipses = true;
          }
        }
        next = m_commands->at(n + 1);
      }
      if (m_hoveringCommand == oldN && !tooltip.empty() && ImGui::BeginTooltip()) {
        ImGui::TextParsed("%s", tooltip.c_str());
        ImGui::EndTooltip();
      }
    }
  }
}
void EventCommandEditor::draw() {

  ImGui::BeginChild("##event_command_editor");
  {
    m_hasFocus = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy);
    ImGui::Text("Content:");
    if (ImGui::BeginTable("##commonevent_code_contents", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                          ImVec2{0, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y})) {
      setupTableColors();
      setupTableHeader();

      const int totalPadding = std::max(static_cast<int>(std::floor(std::log10(m_commands->size()))), 4);
      if (m_commands) {
        ImGui::PushFont(App::APP->getMonoFont());
        for (int n = 0; n < m_commands->size(); n++) {
          const bool isSelected = (m_selectedCommand == n || (m_selectedEnd != -1 && n > m_selectedCommand && n <= m_selectedEnd));
          std::string indentPad = m_commands->at(n)->stringRep(*Database::instance());
          auto str = splitString(indentPad, '\n');
          float height = 0.f;
          for (int i = 0; i < str.size(); ++i) {
            height += ImGui::CalcTextSize(str[i].c_str()).y;
            if (i < str.size() - 1) {
              height += ImGui::GetStyle().ItemSpacing.y;
            }
          }

          ImGui::TableNextRow();
          if (ImGui::TableNextColumn()) {
            const int step = n + 1;
            const int stepPadding = (totalPadding - static_cast<int>(std::floor(std::log10(step)))) + 1;
            if (ImGui::SelectableWithBorder((std::string(stepPadding, ' ') + std::to_string(step)).c_str(), isSelected,
                                            ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick, ImVec2(0, height))) {
              if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) {
                /* If a command has a partner try to flip the selection around, no-op for commands that don't support this */
                if (m_commands->at(n)->hasPartner()) {
                  m_commands->at(n)->setReverseSelection(!m_commands->at(n)->reverseSelection());
                }
              } else if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                m_isRequested = true;
                if (m_commands->at(n)->code() == EventCode::Event_Dummy) {
                  m_isNewEntry = true;
                  ImGui::OpenPopup("Command Window");
                } else {
                  m_isNewEntry = false;
                  /* These are generated automatically based on their related event command dialogs */
                  if (m_commands->at(n)->code() != EventCode::Repeat_Above && m_commands->at(n)->code() != EventCode::Else && m_commands->at(n)->code() != EventCode::End) {
                    commandDialog = CreateCommandDialog(m_commands->at(n)->code(), m_commands->at(n));
                    commandDialog->setOpen(true);
                    // ImGui::OpenPopup("Command Window");
                  }
                }
              }
              if (!ImGui::IsKeyDown(ImGuiKey_LeftShift) && !ImGui::IsKeyDown(ImGuiKey_RightShift)) {
                m_selectedCommand = n;
              }

              if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt) || ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) {
                blockSelect(n);
              } else if (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) {
                if (m_selectedCommand == -1) {
                  m_selectedCommand = n;
                }

                if (n < m_selectedEnd) {
                  m_selectedCommand = n;
                } else {
                  m_selectedEnd = n;
                }
              } else {
                m_selectedEnd = -1;
              }
            }
            if (ImGui::IsItemHovered()) {
              m_hoveringCommand = n;
            }
          }
          if (ImGui::TableNextColumn()) {
            auto cmd = m_commands->at(n);
            if (cmd->collapsable()) {
              if (ImGui::Selectable(std::format("{}##orpg_command_collapse_btn_{}", cmd->isCollapsed() ? "+" : "-", n).c_str(), false, ImGuiSelectableFlags_AllowOverlap)) {
                cmd->setCollapsed(!cmd->isCollapsed());
              }
            } else {
              ImGui::Text(" ");
            }
          }
          if (ImGui::TableNextColumn()) {
            for (const auto& s : str) {
              ImGui::TextParsed("&push-color=0xFF,0xFF,0xFF;%s&pop-color;", s.c_str());
            }
          }

          if (isSelected)
            ImGui::SetItemDefaultFocus();
          handleBlockCollapse(n);
        }
        ImGui::PopFont();
      }
      ImGui::PopStyleColor(5);

      if (ImGui::BeginPopupContextWindow()) {
        if (m_selectedCommand != m_hoveringCommand) {
          m_selectedCommand = m_hoveringCommand;
        }
        if (ImGui::Button("New...", ImVec2(200.0f, 0.0f))) {
          m_isRequested = true;
          m_isNewEntry = true;
        }
        ImGui::EndPopup();
      }
      if (m_isNewEntry)
        ImGui::OpenPopup("Command Window");

      drawPopup();

      // We don't want to draw this if the command window is open, or they'll conflict with each other
      if (!ImGui::IsPopupOpen("Command Window")) {
        drawCommandDialog();
      }

      ImGui::EndTable();
    }
    if (ImGui::IsKeyPressed((ImGuiKey_Delete)) && m_hasFocus) {
      if (m_commands->at(m_selectedCommand)->code() != EventCode::Event_Dummy) {
        int start = m_selectedCommand;
        int end = m_selectedEnd == -1 ? m_selectedCommand + 1 : m_selectedEnd;
        m_commands->erase(m_commands->begin() + start, m_commands->begin() + end);
      }
    }
    handleClipboardInteraction();
  }
  ImGui::EndChild();
}

void EventCommandEditor::drawCommandDialog() {
  if (commandDialog) {
    auto [closed, confirmed] = commandDialog->draw();
    if (!commandDialog->getParentIndent().has_value()) {
      commandDialog->setParentIndent(m_commands->at(m_selectedCommand)->indent.value());
      commandDialog->getCommand()->indent = commandDialog->getParentIndent().value();
    }
    if (closed) {
      if (confirmed) {
        if (m_isNewEntry) {
          std::vector<std::shared_ptr<IEventCommand>> cmds = commandDialog->getBatchCommands();
          if (cmds.empty()) {
            auto select = m_commands->insert(m_commands->begin() + m_selectedCommand, commandDialog->getCommand());
            m_selectedCommand = select - m_commands->begin();
            m_selectedEnd = -1;
          } else {
            for (auto cmd : cmds) {
              auto selection = m_commands->insert(m_commands->begin() + m_selectedCommand, cmd);
              m_selectedCommand = (selection + 1) - m_commands->begin();
            }
          }
          m_isNewEntry = false;
        } else {
          if (std::dynamic_pointer_cast<ShowChoiceCommand>(commandDialog->getCommand())) {
            std::shared_ptr<ShowChoiceCommand> commandPointer = std::dynamic_pointer_cast<ShowChoiceCommand>(commandDialog->getCommand());

            for (auto choice : commandPointer->choices) {
              APP_INFO("Choice Print: " + choice);
            }
            EventCode code = commandDialog->getCommand()->code();
            int whenIndex{0};
            int deletionIndex{0};
            bool isDeletion{false};
            int choiceSize = commandPointer->choices.size();
            APP_INFO(std::to_string(choiceSize));
            for (int i = m_selectedCommand; i < m_commands->size(); i++) {
              if (m_commands->at(i)->code() == EventCode::End_del_ShowChoices) {
                if (choiceSize > whenIndex) {
                  int numberOfWhens = choiceSize - whenIndex;
                  APP_INFO("Found more options... need to make more whens: " + std::to_string(numberOfWhens));
                  for (int z{0}; z < numberOfWhens; z++) {
                    for (auto cmd : commandDialog->getTemplateCommands(EventCode::When_Selected, whenIndex)) {
                      m_commands->insert(m_commands->begin() + (i - 1), cmd);
                    }
                  }
                }

                if (isDeletion) {
                  APP_INFO("isDeletion...");
                  APP_INFO("Deletion index... " + std::to_string(deletionIndex));
                  APP_INFO(DecodeEnumName(m_commands->at(deletionIndex)->code()));
                  APP_INFO(DecodeEnumName(m_commands->at(i - 1)->code()));
                  m_commands->erase(m_commands->begin() + (deletionIndex - 1), m_commands->begin() + (i - 1));
                  break;
                }
              }

              if (m_commands->at(i)->code() == EventCode::When_Selected) {
                if (whenIndex < commandPointer->choices.size()) {
                  std::shared_ptr<WhenSelectedCommand> when = std::static_pointer_cast<WhenSelectedCommand>(m_commands->at(i));

                  when->choice = commandPointer->choices.at(whenIndex);
                  when->param1 = whenIndex;

                  whenIndex++;
                } else {
                  if (!isDeletion) {
                    isDeletion = true;
                    deletionIndex = i;
                  }
                }
              }
            }
          }
        }
      }
      commandDialog.reset();
      ImGui::CloseCurrentPopup();
    }
  }
}
void EventCommandEditor::drawSystemTab(ImVec2 size) {
  if (ImGui::BeginTabItem("System")) {
    if (ImGui::Button("Open Menu Screen", size)) {
      commandDialog = CreateCommandDialog(EventCode::Open_Menu_Screen);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Change Battle BGM...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Battle_BGM);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third column
    if (ImGui::Button("Script...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Script);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Open Save Screen", size)) {
      commandDialog = CreateCommandDialog(EventCode::Open_Save_Screen);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Change Vehicle BGM...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Vechicle_BGM);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third column
    if (ImGui::Button("Plugin Command...", size)) {
      commandDialog = CreateCommandDialog(EventCode::PluginMV_Command);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Menu Access...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Menu_Access);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Change Victory ME...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Victory_ME);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third column
    if (ImGui::Button("Game Over", size)) {
      commandDialog = CreateCommandDialog(EventCode::Game_Over);
    }
    if (ImGui::Button("Change Save Access...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Save_Access);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Change Defeat ME...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Defeat_ME);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third column
    if (ImGui::Button("Return To Title Screen", size)) {
      commandDialog = CreateCommandDialog(EventCode::Return_To_Title_Screen);
    }
    if (ImGui::Button("Change Map Display Name...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Map_Name_Display);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Change Parallax...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Parallax);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Tileset...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Tileset);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Change Actor Images...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Actor_Images);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Battle Back...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Battle_Back);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Change Vehicle Image...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Vehicle_Image);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Player Followers...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Player_Followers);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Change Window Color...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Window_Color);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Formation Access...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Formation_Access);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Change Encounter...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Encounter_Disable);
      commandDialog->setOpen(true);
    }
    ImGui::EndTabItem();
  }
}
void EventCommandEditor::drawScreenTab(ImVec2 size) {
  if (ImGui::BeginTabItem("Screen")) {
    if (ImGui::Button("Show Picture...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Show_Picture);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Tint Screen...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Tint_Screen);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third column
    if (ImGui::Button("Set Weather Effect...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Set_Weather_Effect);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Move Picture...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Move_Picture);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Flash Screen...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Flash_Screen);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third column
    if (ImGui::Button("Scroll Map...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Scroll_Map);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Rotate Picture...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Rotate_Picture);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Shake Screen...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Shake_Screen);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Tint Picture...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Tint_Picture);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Fadeout Screen...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Fadeout_Screen);
    }
    if (ImGui::Button("Erase Picture...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Erase_Picture);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second column
    if (ImGui::Button("Fadein Screen...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Fadein_Screen);
    }
    ImGui::EndTabItem();
  }
}
void EventCommandEditor::drawSceneTab(ImVec2 size) {
  if (ImGui::BeginTabItem("Scene")) {
    if (ImGui::Button("Show Text...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Show_Text);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Show Animation...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Show_Animation);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third Column
    if (ImGui::Button("Change Transparency...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Transparency);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Show Choices...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Show_Choices);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Show Battle Animation...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Show_Battle_Animation);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third Column
    if (ImGui::Button("Get on/off Vehicle", size)) {
      commandDialog = CreateCommandDialog(EventCode::Get_On_Off_Vehicle);
    }
    if (ImGui::Button("Show Scrolling Text...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Show_Scrolling_Text);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Show Balloon Icon...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Show_Balloon_Icon);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third Column
    if (ImGui::Button("Abort Battle", size)) {
      commandDialog = CreateCommandDialog(EventCode::Abort_Battle);
    }
    if (ImGui::Button("Input Number...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Input_Number);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Shop Processing...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Shop_Processing);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third Column
    if (ImGui::Button("Gather Followers", size)) {
      commandDialog = CreateCommandDialog(EventCode::Gather_Followers);
    }
    if (ImGui::Button("Select Item...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Select_Item);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Battle Processing...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Battle_Processing);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Set Movement Route...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Set_Movement_Route);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Name Input Processing...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Name_Input_Processing);
      commandDialog->setOpen(true);
    }
    ImGui::EndTabItem();
  }
}
void EventCommandEditor::drawFlowControlTab(ImVec2 size) {
  if (ImGui::BeginTabItem("Flow")) {
    if (ImGui::Button("Conditional Branch...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Conditional_Branch);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Control Switches...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Control_Switches);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third Column
    if (ImGui::Button("Wait...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Wait);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Loop", size)) {
      commandDialog = CreateCommandDialog(EventCode::Loop);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Control Variables...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Control_Variables);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third Column
    if (ImGui::Button("Erase Event", size)) {
      commandDialog = CreateCommandDialog(EventCode::Erase_Event);
    }
    if (ImGui::Button("Break Loop", size)) {
      commandDialog = CreateCommandDialog(EventCode::Break_Loop);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Control Self Switch...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Control_Self_Switch);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Third Column
    if (ImGui::Button("Exit Event Processing", size)) {
      commandDialog = CreateCommandDialog(EventCode::Exit_Event_Processing);
    }
    if (ImGui::Button("Common Event...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Common_Event);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Control Timer...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Control_Timer);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Label...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Label);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Transfer Player...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Transfer_Player);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Jump to Label...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Jump_To_Label);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Set Event Location...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Set_Event_Location);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Comment...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Comment);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine(); // Second Column
    if (ImGui::Button("Set Vehicle Location...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Set_Vehicle_Location);
      commandDialog->setOpen(true);
    }
    ImGui::EndTabItem();
  }
}
void EventCommandEditor::drawAudioTab(ImVec2 size) {
  if (ImGui::BeginTabItem("Audio")) {
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Play BGM...", size)) {
        commandDialog = CreateCommandDialog(EventCode::Play_BGM);
        commandDialog->setOpen(true);
      }
      if (ImGui::Button("Save BGM", size)) {
        commandDialog = CreateCommandDialog(EventCode::Save_BGM);
      }
      if (ImGui::Button("Fadeout BGM...", size)) {
        commandDialog = CreateCommandDialog(EventCode::Fadeout_BGM);
        commandDialog->setOpen(true);
      }
      if (ImGui::Button("Replay BGM", size)) {
        commandDialog = CreateCommandDialog(EventCode::Resume_BGM);
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Play BGS...", size)) {
        commandDialog = CreateCommandDialog(EventCode::Play_BGS);
        commandDialog->setOpen(true);
      }
      if (ImGui::Button("Fadeout BGS...", size)) {
        commandDialog = CreateCommandDialog(EventCode::Fadeout_BGS);
        commandDialog->setOpen(true);
      }
      if (ImGui::Button("Play Movie...", size)) {
        commandDialog = CreateCommandDialog(EventCode::Play_Movie);
        commandDialog->setOpen(true);
      }
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Play ME...", size)) {
        commandDialog = CreateCommandDialog(EventCode::Play_ME);
        commandDialog->setOpen(true);
      }
      if (ImGui::Button("Play SE...", size)) {
        commandDialog = CreateCommandDialog(EventCode::Play_SE);
        commandDialog->setOpen(true);
      }
      if (ImGui::Button("Stop SE", size)) {
        commandDialog = CreateCommandDialog(EventCode::Stop_SE);
      }
    }
    ImGui::EndGroup();
    ImGui::EndTabItem();
  }
}
void EventCommandEditor::drawActorTab(ImVec2 size) {
  if (ImGui::BeginTabItem("Actor")) {
    if (ImGui::Button("Change HP...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_HP);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Gold...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Gold);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Enemy HP...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Enemy_HP);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change MP...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_MP);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Items...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Items);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Enemy MP...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Enemy_MP);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change TP...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_TP);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Weapons...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Weapons);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Enemy TP...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Enemy_TP);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change State...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_State);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Armors...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Armors);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Enemy State...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Enemy_State);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Recover All...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Recover_All);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Party Member...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Party_Member);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Enemy Recover All...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Enemy_Recover_All);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change EXP...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_EXP);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Nickname...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Nickname);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Enemy Appear...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Enemy_Appear);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Level...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Level);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Change Profile...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Profile);
      commandDialog->setOpen(true);
    }
    ImGui::SameLine();
    if (ImGui::Button("Enemy Transform...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Enemy_Transform);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Parameter...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Parameter);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Skill...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Skill);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Equipment...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Equipment);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Name...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Name);
      commandDialog->setOpen(true);
    }
    if (ImGui::Button("Change Class...", size)) {
      commandDialog = CreateCommandDialog(EventCode::Change_Class);
      commandDialog->setOpen(true);
    }
    ImGui::EndTabItem();
  }
}
void EventCommandEditor::drawPopup() {
  if (m_selectedCommand < 0) {
    return;
  }
  if (!m_isRequested) {
    return;
  }

  ImGui::SetNextWindowSize(ImVec2{680, 550} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal("Command Window", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
    ImGui::BeginGroup();
    {

      if (ImGui::BeginTabBar("##orpg_command_window")) {

        ImVec2 size = ImVec2{((ImGui::GetContentRegionAvail().x / 2) / 2) + App::DPIHandler::scale_value(50), 0};

        drawActorTab(size);
        drawAudioTab(size);
        drawFlowControlTab(size);
        drawSceneTab(size);
        drawScreenTab(size);
        drawSystemTab(size);

        // Event Templates
        ImGui::PushItemWidth(App::DPIHandler::scale_value(500));
        if (ImGui::BeginCombo("##eventcommand_editor_presets", "Select a preset to insert into selection...")) {
          int index{0};
          for (auto& templ : Database::instance()->templates.templates) {
            if (templ.type() == Template::TemplateType::Command) {
              if (!templ.commands().empty()) {
                if (ImGui::Selectable(templ.name().c_str(), false)) {

                  CommandParser parser;
                  nlohmann::ordered_json cmdJson = nlohmann::ordered_json::parse(templ.commands());
                  std::vector<std::shared_ptr<IEventCommand>> parsed = parser.parse(cmdJson);
                  for (const auto& command : parsed) {
                    if (command) {
                      command->adjustIndent(m_commands->at(m_selectedCommand)->indent.value());
                    }
                  }
                  if ((parsed.size() == 1 && parsed.at(0)->code() == EventCode::Event_Dummy) == false) {
                    m_commands->insert(m_commands->begin() + m_selectedCommand, parsed.begin(), parsed.end() - 1);
                  }
                  m_isNewEntry = false;
                  m_isRequested = false;
                  // ImGui::CloseCurrentPopup();
                }
                index++;
              } else {
                if (ImGui::Selectable(("Error loading template: " + templ.name()).c_str(), false)) {}
              }
            }
          }
          ImGui::EndCombo();
        }

        drawCommandDialog();

        ImGui::EndTabBar();
      }
    }
    ImGui::EndGroup();

    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      if (commandDialog) {
        commandDialog.reset();
      } else {
        m_isNewEntry = false;
        m_selectedCommand = 0;
        ImGui::CloseCurrentPopup();
      }
    }

    ImGui::EndPopup();
  } // End of "Command Window" Popup
}
