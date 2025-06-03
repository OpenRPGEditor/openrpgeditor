#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/ImGuiExt/ImGuiParsedText.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include "Core/Application.hpp"

#include "Core/Log.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <clip.h>
#include <list>

#include <Core/Utils.hpp>
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
void EventCommandEditor::blockSelect(const int n, const bool isDelete = false) {
  if (m_commands->at(n)->hasPartner()) {
    if (!m_commands->at(n)->reverseSelection()) {
      int j = n + 1;
      int partnerCount = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl) || m_commands->at(n)->isCollapsed() ? m_commands->at(n)->partnerCount() : 1;

      if (isDelete) {
        partnerCount = m_commands->at(n)->partnerCount();
      }
      while (partnerCount) {
        if (j >= m_commands->size() || *m_commands->at(j)->indent() < *m_commands->at(n)->indent()) {
          break;
        }

        while (true) {
          if (j >= m_commands->size() || m_commands->at(n)->isPartner(m_commands->at(j)->code(), m_commands->at(j)->indent())) {
            partnerCount--;
            if (m_commands->at(n)->isTerminatingPartner(m_commands->at(j)->code(), m_commands->at(j)->indent())) {
              break;
            }
            if (partnerCount > 0) {
              ++j;
            }
            break;
          }
          ++j;
        }
        if (j >= m_commands->size() - 1 || (m_commands->at(j) && m_commands->at(j)->code() == EventCode::Event_Dummy)) { // ???
          j++;
        }
      }
      m_selectedEnd = j;
    } else {
      m_selectedEnd = n;
      int j = n - 1;
      int partnerCount = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl) ? m_commands->at(n)->partnerCount() : 1;
      while (partnerCount--) {
        if (j < 0 || *m_commands->at(j)->indent() < *m_commands->at(n)->indent()) {
          if (*m_commands->at(j)->indent() < *m_commands->at(n)->indent()) {
            ++j;
          }
          break;
        }
        while (true) {
          if (j < 0 || m_commands->at(n)->isPartner(m_commands->at(j)->code(), m_commands->at(j)->indent())) {
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

void EventCommandEditor::handleClipboardInteraction() {
  if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy)) {
    return;
  }

  if (RPGMVEventCommandFormat == -1) {
    RPGMVEventCommandFormat = clip::register_format("application/ore-EventCommand");
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
        int curIndent = m_selectedCommand > 0 ? *m_commands->at(m_selectedCommand)->indent() : 0;
        int nestedCount = 0;
        if (isNestable(commands[0])) {
          ++nestedCount;
        }

        for (auto& command : commands) {
          if (!command->indent()) {
            command->setIndent(0);
          }
          if (isNestableEnd(command)) {
            if (nestedCount > 0 && curIndent > 0) {
              curIndent--;
            }
            nestedCount--;
          }
          command->setIndent(curIndent);
          if (isNestable(command)) {
            curIndent++;
            nestedCount++;
          }
        }
        m_commands->insert(m_commands->begin() + m_selectedCommand, commands.begin(), commands.end());
      }
    }
  }
  if (ImGui::IsKeyPressed(ImGuiKey_C) && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))) {
    clip::lock l;
    int start = m_selectedCommand;
    int end = m_selectedEnd == -1 ? m_selectedCommand + 1 : m_selectedEnd + 1;
    std::vector<std::shared_ptr<IEventCommand>> commands(m_commands->begin() + start, m_commands->begin() + end);
    nlohmann::ordered_json cmdJson;
    CommandParser::serialize(cmdJson, commands);
    auto v = cmdJson.dump();
    l.set_data(RPGMVEventCommandFormat, v.data(), v.size());
  }
  if (ImGui::IsKeyPressed(ImGuiKey_X) && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))) {
    if (m_commands->at(m_selectedCommand)->code() != EventCode::Event_Dummy) {
      clip::lock l;
      int start = m_selectedCommand;
      int end = m_selectedEnd == -1 ? m_selectedCommand + 1 : m_selectedEnd + 1;
      std::vector<std::shared_ptr<IEventCommand>> commands(m_commands->begin() + start, m_commands->begin() + end);
      nlohmann::ordered_json cmdJson;
      CommandParser::serialize(cmdJson, commands);
      auto v = cmdJson.dump();
      l.set_data(RPGMVEventCommandFormat, v.data(), v.size());
      if (m_commands->at(m_selectedCommand)->isParent()) {
        blockSelect(m_selectedCommand);
        m_commands->erase(m_commands->begin() + m_selectedCommand, m_commands->begin() + m_selectedEnd + 1);
        m_selectedEnd = -1;
      } else {
        m_commands->erase(m_commands->begin() + start, m_commands->begin() + end);
      }
    }
  }
}
void EventCommandEditor::setupTableHeader() {
  ImGui::TableSetupColumn("Step##command_selectable_column", ImGuiTableColumnFlags_WidthFixed);
  ImGui::TableSetupColumn("##collapse", ImGuiTableColumnFlags_WidthFixed);
  ImGui::TableSetupColumn("Command Operation", ImGuiTableColumnFlags_WidthFixed);
  ImGui::TableSetupScrollFreeze(2, 1);
  ImGui::TableHeadersRow();
}

void EventCommandEditor::setupTableColors() {
  // ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.37f, 0.37f, 0.37f, 0.43f));
  // ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.37f, 0.37f, 0.37f, 0.78f));
  // ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.37f, 0.37f, 0.37f, 0.68f));
  // ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(0.03f, 0.21f, 0.26f, 1.00f));
  // ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(0.04f, 0.16f, 0.19f, 1.00f));
}
void EventCommandEditor::handleBlockCollapse(int& n) const {
  if (m_commands->at(n)->collapsable()) {
    const int oldN = n;
    if (const auto& cmd = m_commands->at(n); cmd->isCollapsed()) {
      std::vector<std::string> tooltip;
      auto next = m_commands->at(n + 1);
      const int indent = next->indent().value_or(0);
      bool addedElipses = false;
      while (!cmd->isPartner(next->code(), next->indent())) {
        if ((n + 1) >= m_commands->size() - 1) {
          break;
        }
        n++;

        if (next->code() != EventCode::Event_Dummy) {
          if (tooltip.size() < 5) {
            auto splitRep = splitString(next->stringRep(*Database::instance(), true), '\n');
            for (std::string s : splitRep) {
              if (tooltip.size() >= 5) {
                break;
              }
              tooltip.push_back(s.erase(0, indent * 2));
            }
          } else if (!addedElipses) {
            auto tmp = *next->indent() - indent;
            // If we're at the same indent we don't want to drop down a level
            if (tmp <= 0) {
              tmp = indent;
            }
            tooltip.push_back(next->indentText(tmp) + "\u2026");
            addedElipses = true;
          }
        }
        next = m_commands->at(n + 1);
      }
      ImGui::SetNextWindowSizeConstraints({}, ImGui::GetDPIScaledSize(640, 640));
      if (m_hoveringCommand == oldN && !tooltip.empty() && ImGui::BeginTooltipEx(ImGuiTooltipFlags_None, ImGuiWindowFlags_NoScrollbar)) {
        ImGui::TextParsed("%s", std::accumulate(std::next(tooltip.begin()), tooltip.end(), tooltip[0], [](const std::string& a, const std::string& b) { return a + "\n" + b; }).c_str());
        ImGui::EndTooltip();
      }
    }
  }
}
void EventCommandEditor::draw() {
  if (!ImGui::IsWindowHovered()) {
    // m_hoveringCommand = -1; -- Not working. This causes the PopupContextMenu to deselect and cause issues.
  }

  ImGui::BeginChild("##event_command_editor");
  {
    ImGui::Text("Content:");
    ImGui::PushFont(App::APP->getMonoFont(), Settings::instance()->monoFontSize);
    if (ImGui::BeginTable("##commonevent_code_contents", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, ImGui::GetContentRegionAvail())) {
      setupTableColors();
      setupTableHeader();
      const int totalPadding = std::max(static_cast<int>(std::floor(std::log10(m_commands->size()))), 3);
      if (m_commands) {
        float maxWidth = 0.f;
        // Unfortunately ImGui's table doesn't take into account all items, only the visible ones, we want to take into account all commands so the table doesn't jitter around as the user scrolls
        // vertically, this is a bit expensive, but doesn't seem to greatly impact performance, if there is a better way in the future this should be removed/replaced
        for (const auto& cmd : *m_commands) {
          if (!cmd) {
            continue;
          }
          maxWidth = std::max(maxWidth, ImGui::CalcItemSize(ImGui::CalcTextSize(cmd->stringRep(*Database::instance(), false).c_str()), 0.f, 0.f).x);
        }

        for (int n = 0; n < m_commands->size(); n++) {
          if (!m_commands->at(n)) {
            continue;
          }
          switch (const auto& cmd = m_commands->at(n); cmd->code()) {
          case EventCode::Enemy_Recover_All:
            std::dynamic_pointer_cast<EnemyRecoverAllCommand>(cmd)->m_troopId = m_troopId;
            break;
          case EventCode::Enemy_Appear:
            std::dynamic_pointer_cast<EnemyAppearCommand>(cmd)->m_troopId = m_troopId;
            break;
          case EventCode::Enemy_Transform:
            std::dynamic_pointer_cast<EnemyTransformCommand>(cmd)->m_troopId = m_troopId;
            break;
          case EventCode::Change_Enemy_HP:
            std::dynamic_pointer_cast<ChangeEnemyHPCommand>(cmd)->m_troopId = m_troopId;
            break;
          case EventCode::Change_Enemy_MP:
            std::dynamic_pointer_cast<ChangeEnemyMPCommand>(cmd)->m_troopId = m_troopId;
            break;
          case EventCode::Change_Enemy_TP:
            std::dynamic_pointer_cast<ChangeEnemyTPCommand>(cmd)->m_troopId = m_troopId;
            break;
          case EventCode::Change_Enemy_State:
            std::dynamic_pointer_cast<ChangeEnemyStateCommand>(cmd)->m_troopId = m_troopId;
            break;
          case EventCode::Conditional_Branch:
            std::dynamic_pointer_cast<ConditionalBranchCommand>(cmd)->m_troopId = m_troopId;
            break;
          case EventCode::Control_Variables:
            std::dynamic_pointer_cast<ControlVariables>(cmd)->m_troopId = m_troopId;
            break;
          default:
            break;
          }
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
            const int stepPadding = (totalPadding - static_cast<int>(std::floor(std::log10(step))));
            if (ImGui::SelectableWithBorder((std::string(stepPadding, ' ') + std::to_string(step)).c_str(), isSelected,
                                            ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap,
                                            ImVec2(0, height))) {

              if (ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightCtrl)) {
                /* If a command has a partner try to flip the selection around, no-op for commands that don't support this */
                if (m_commands->at(n)->hasPartner()) {
                  m_commands->at(n)->setReverseSelection(!m_commands->at(n)->reverseSelection());
                }
              } else if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                m_isRequested = true;
                if (m_commands->at(n)->code() == EventCode::Event_Dummy) {
                  m_isNewEntry = true;
                  ImGui::OpenPopup("###command_picker");
                } else {
                  m_isNewEntry = false;
                  /* These are generated automatically based on their related event command dialogs */
                  if (m_commands->at(n)->code() != EventCode::Repeat_Above && m_commands->at(n)->code() != EventCode::When_Selected && m_commands->at(n)->code() != EventCode::When_Cancel &&
                      m_commands->at(n)->code() != EventCode::Else && m_commands->at(n)->code() != EventCode::End) {
                    m_commandDialog = CreateCommandDialog(m_commands->at(n)->code(), m_commands->at(n));
                    if (m_commands->at(n)->code() == EventCode::Conditional_Branch) {
                      blockSelect(n, true);
                      for (int i = n; i < m_selectedEnd; i++) {
                        if (m_commands->at(n)->isPartner(m_commands->at(i)->code(), m_commands->at(n)->indent())) {
                          m_commandDialog->setElse(true); // Has an else, set to true.
                        }
                      }
                    }
                    m_commandDialog->setOpen(true);
                    // ImGui::OpenPopup("###command_picker");
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
              if (m_commands->at(n)->code() == EventCode::Show_Choices) {
                blockSelect(n);
              }
            }
            if (ImGui::IsItemHovered()) {
              m_hoveringCommand = n;
            }
          }
          if (ImGui::TableNextColumn()) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            auto size = ImGui::CalcTextSize("+");
            size.x = size.y;
            size = ImGui::CalcItemSize(size, 0, 0);
            if (const auto cmd = m_commands->at(n); cmd->collapsable()) {
              if (ImGui::Button(std::format("{}##orpg_command_collapse_btn_{}", cmd->isCollapsed() ? "+" : "-", n).c_str(), size)) {
                cmd->setCollapsed(!cmd->isCollapsed());
              }
            } else {
              /* Use a dummy to keep consistent sizing */
              ImGui::Dummy(size);
            }
            ImGui::PopStyleVar();
          }
          if (ImGui::TableNextColumn()) {
            for (const auto& s : str) {
              // Hack to keep scrollbar sizing consistent
              const auto oldPos = ImGui::GetCursorPos();
              ImGui::Dummy({maxWidth, 1.f});
              ImGui::SetCursorPos(oldPos);
              ImGui::TextParsed("%s", s.c_str());
            }
          }

          if (isSelected)
            ImGui::SetItemDefaultFocus();
          handleBlockCollapse(n);
        }
      }
      ImGui::PopFont();
      // ImGui::PopStyleColor(5);

      if (ImGui::BeginPopupContextWindow()) {
        if (m_selectedCommand != m_hoveringCommand) {
          m_selectedCommand = m_hoveringCommand;
        }
        if (ImGui::MenuItem(trNOOP("New..."))) {
          m_isRequested = true;
          m_isNewEntry = true;
        }
        ImGui::EndPopup();
      }
      if (m_isNewEntry)
        ImGui::OpenPopup("###command_picker");

      drawPopup();

      // We don't want to draw this if the command window is open, or they'll conflict with each other
      if (!ImGui::IsPopupOpen("###command_picker")) {
        drawCommandDialog();
      }

      ImGui::EndTable();
    } else {
      ImGui::PopFont();
    }
    if (ImGui::IsKeyPressed((ImGuiKey_Delete)) && ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy)) {
      if (m_commands->at(m_selectedCommand)->isParent()) {
        blockSelect(m_selectedCommand, true);
        m_commands->erase(m_commands->begin() + m_selectedCommand, m_commands->begin() + m_selectedEnd + 1);
        m_selectedEnd = -1;
      } else {
        if (m_commands->at(m_selectedCommand)->code() != EventCode::Event_Dummy) {
          int start = m_selectedCommand;
          int end = m_selectedEnd == -1 ? m_selectedCommand + 1 : m_selectedEnd + 1;
          m_commands->erase(m_commands->begin() + start, m_commands->begin() + end);
          m_selectedEnd = -1;
        }
      }
    }
  }
  ImGui::EndChild();
  handleClipboardInteraction();
}

void EventCommandEditor::drawCommandDialog() {
  if (m_commandDialog) {
    auto [closed, confirmed] = m_commandDialog->draw();
    if (!m_commandDialog->getParentIndent().has_value()) {
      m_commandDialog->setParentIndent(m_commands->at(m_selectedCommand)->indent().value());
      m_commandDialog->getCommand()->setIndent(m_commandDialog->getParentIndent().value());
    }
    if (closed) {
      if (confirmed) {
        if (m_isNewEntry) {
          std::vector<std::shared_ptr<IEventCommand>> cmds = m_commandDialog->getBatchCommands();
          if (cmds.empty()) {
            auto select = m_commands->insert(m_commands->begin() + m_selectedCommand, m_commandDialog->getCommand());
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
          if (m_commandDialog->getCommand()->code() == EventCode::Show_Choices) {
            std::shared_ptr<ShowChoiceCommand> pointerCmd = std::dynamic_pointer_cast<ShowChoiceCommand>(m_commandDialog->getCommand());
            // std::shared_ptr<ShowChoiceCommand> selectedCmd = std::dynamic_pointer_cast<ShowChoiceCommand>(m_commands->at(m_selectedCommand));

            int choicesTotal{0};
            int totalChoices{0};
            int m_choiceEnd{0};
            int m_choiceStart{0};
            std::vector<std::shared_ptr<IEventCommand>> whenList;
            std::vector<std::shared_ptr<IEventCommand>> cmdList;
            std::map<int, std::vector<std::shared_ptr<IEventCommand>>> tmp;

            // std::vector<std::string> listChoices;
            std::vector<std::string> finalChoices;

            for (int i = m_selectedCommand + 1; i < m_commands->size(); i++) {
              if (m_commands->at(i)->code() == EventCode::When_Selected && m_commands->at(i)->indent() == pointerCmd->indent().value()) {
                choicesTotal++;
              } else if (m_commands->at(i)->code() == EventCode::When_Cancel && m_commands->at(i)->indent() == pointerCmd->indent().value()) {
                choicesTotal++;
              }
            }

            int choiceDifference = pointerCmd->choices.size() - choicesTotal;
            choiceDifference = abs(choiceDifference);

            bool cancelFound{false};
            bool cancelCreate{false};
            bool cancelDelete{false};
            bool choiceEnd{false};
            if (choiceDifference == 0) {
              // just update the names
              int updateIndex{0};
              for (int i = m_selectedCommand + 1; i < m_commands->size(); i++) {
                if (!choiceEnd) {
                  if (m_commands->at(i)->code() == EventCode::End_del_ShowChoices && m_commands->at(i)->indent() == pointerCmd->indent().value()) {
                    choiceEnd = true;
                  }
                  if (m_commands->at(i)->code() == EventCode::When_Selected && m_commands->at(i)->indent() == pointerCmd->indent().value()) {
                    std::shared_ptr<WhenSelectedCommand> cmd = std::dynamic_pointer_cast<WhenSelectedCommand>(m_commands->at(i));
                    cmd->choice = pointerCmd->choices.at(updateIndex);
                    updateIndex++;
                  }
                  if (m_commands->at(i)->code() == EventCode::When_Cancel && m_commands->at(i)->indent() == pointerCmd->indent().value()) {
                    cancelFound = true;
                  }
                  if (pointerCmd->cancelType == -2) {
                    if (cancelFound) {
                      // Nothing
                    } else {
                      // No cancel and cancelType is set. We need to make a cancel
                      cancelCreate = true;
                    }
                  } else {
                    if (cancelFound) {
                      // Needs to be removed
                      cancelDelete = true;
                    } else {
                      // Nothing
                    }
                  }
                  m_choiceEnd = i;
                }
              }
              if (cancelCreate) {
                std::shared_ptr<EventDummy> dummy = std::make_shared<EventDummy>();
                dummy->setIndent(pointerCmd->indent().value() + 1);
                m_commands->insert(m_commands->begin() + m_choiceEnd, dummy);

                std::shared_ptr<WhenCancelCommand> whenCmd = std::make_shared<WhenCancelCommand>();
                whenCmd->setIndent(m_commandDialog->getParentIndent().value());
                m_commands->insert(m_commands->begin() + m_choiceEnd, whenCmd);
              } else if (cancelDelete) {
                m_commands->erase(m_commands->begin() + m_choiceStart, m_commands->begin() + m_choiceEnd - 1);
              }
            } else {
              for (int i = m_selectedCommand + 1; i < m_commands->size(); i++) {
                if (!choiceEnd) {
                  if (m_commands->at(i)->code() == EventCode::End_del_ShowChoices && m_commands->at(i)->indent() == pointerCmd->indent().value()) {
                    choiceEnd = true;
                  }
                  if (m_commands->at(i)->code() == EventCode::When_Selected && m_commands->at(i)->indent() == pointerCmd->indent().value()) {
                    std::shared_ptr<WhenSelectedCommand> cmd = std::dynamic_pointer_cast<WhenSelectedCommand>(m_commands->at(i));
                    if (!whenList.empty()) {
                      tmp.insert(std::make_pair(totalChoices, whenList)); // Adds any existing when commands into tmp as index
                      whenList.clear();
                      totalChoices++;
                    }
                    // listChoices.push_back(cmd->choice);
                  } else if (m_commands->at(i)->code() == EventCode::When_Cancel && m_commands->at(i)->indent() == pointerCmd->indent().value()) {
                    if (!whenList.empty()) {
                      tmp.insert(std::make_pair(totalChoices, whenList)); // Adds any existing when commands into tmp as index
                      whenList.clear();
                      totalChoices++;
                    }
                    // listChoices.push_back("\\CANCEL");
                  } else {
                    if (!choiceEnd) {
                      whenList.push_back(m_commands->at(i));
                    }
                  }
                  m_choiceEnd = i;
                }
              }
              if (!whenList.empty()) {
                tmp.insert(std::make_pair(totalChoices, whenList)); // Adds any existing when commands into tmp as index
                whenList.clear();
              }

              // Construct the final show choice branches
              cmdList.push_back(pointerCmd);
              int tempIndex{0};
              for (auto& str : pointerCmd->choices) {
                std::shared_ptr<WhenSelectedCommand> whenCmd = std::make_shared<WhenSelectedCommand>();
                whenCmd->setIndent(m_commandDialog->getParentIndent().value());
                whenCmd->choice = str;
                cmdList.insert(cmdList.end(), whenCmd);
                if (tempIndex < tmp.size()) {
                  for (auto& listCmd : tmp[tempIndex]) {
                    cmdList.insert(cmdList.end(), listCmd);
                  }
                } else {
                  // Insert event dummy, no commands
                  std::shared_ptr<EventDummy> dummy = std::make_shared<EventDummy>();
                  dummy->setIndent(pointerCmd->indent().value() + 1);
                  cmdList.insert(cmdList.end(), dummy);
                }
                tempIndex++;
              }
              if (pointerCmd->cancelType == -2) {
                std::shared_ptr<WhenCancelCommand> endCmd = std::make_shared<WhenCancelCommand>();
                endCmd->setIndent(m_commandDialog->getParentIndent().value());
                cmdList.push_back(endCmd);
                if (tempIndex < tmp.size()) {
                  for (auto& listCmd : tmp[tempIndex]) {
                    cmdList.insert(cmdList.end(), listCmd);
                  }
                } else {
                  // Insert event dummy, no commands
                  std::shared_ptr<EventDummy> dummy = std::make_shared<EventDummy>();
                  dummy->setIndent(pointerCmd->indent().value() + 1);
                  cmdList.insert(cmdList.end(), dummy);
                }
              }

              std::shared_ptr<ShowChoicesEndCommand> endCmd = std::make_shared<ShowChoicesEndCommand>();
              endCmd->setIndent(m_commandDialog->getParentIndent().value());
              cmdList.push_back(endCmd);

              m_commands->erase(m_commands->begin() + m_selectedCommand, m_commands->begin() + m_choiceEnd + 1);
              m_commands->insert(m_commands->begin() + m_selectedCommand, cmdList.begin(), cmdList.end());
            }
          }
          if (m_commandDialog->getCommand()->code() == EventCode::Conditional_Branch) {
            if (!m_commandDialog->isCurrentElseBranch()) {
              std::vector<std::shared_ptr<IEventCommand>> tempCmds;

              for (int i = m_selectedCommand + 1; i < m_commands->size(); i++) {
                if (m_commands->at(m_selectedCommand)->isTerminatingPartner(m_commands->at(i)->code(), m_commands->at(i)->indent())) {
                  break;
                }
                if (m_commands->at(i)->code() == EventCode::Else) {
                  break;
                }
                tempCmds.push_back(m_commands->at(i));
              }
              std::vector<std::shared_ptr<IEventCommand>> commandCmds = m_commandDialog->getBatchCommands(tempCmds);
              blockSelect(m_selectedCommand, true);
              m_commands->erase(m_commands->begin() + m_selectedCommand, m_commands->begin() + m_selectedEnd + 1);
              m_commands->insert(m_commands->begin() + m_selectedCommand, commandCmds.begin(), commandCmds.end());
              // Delete branch, store contents, redraw and insert
              m_commandDialog->setCurrentElseBranch();
              m_selectedEnd = -1;
            }
          }
        }

        if (m_commands->at(m_selectedCommand)->code() == EventCode::Set_Movement_Route) {
          std::shared_ptr<SetMovementRouteCommand> routeCmd = std::dynamic_pointer_cast<SetMovementRouteCommand>(m_commands->at(m_selectedCommand));

          routeCmd->editNodes.clear();
          for (const auto& cmd : routeCmd->route.list()) {
            if (cmd->code() != EventCode::Event_Dummy) {
              routeCmd->editNodes.emplace_back(std::make_shared<MovementRouteStepCommand>(m_commands->at(m_selectedCommand)->indent()));
              routeCmd->editNodes.back()->step = cmd;
            }
          }
        }
      }
      m_commandDialog.reset();
      ImGui::CloseCurrentPopup();
    }
  }
}

void EventCommandEditor::drawCommandTab(const EventCommandTab& tab, const std::string& id, ImVec2 size) {
  if (ImGui::BeginTabItem(std::format("{}###{}", tab.title, id).c_str())) {
    ImGui::BeginHorizontal(std::format("commandDialog_{}_horizontal", tab.title).c_str(), {-1, -1});
    {
      for (int i = 0; const auto& column : tab.buttonColumns) {
        ImGui::BeginVertical(std::format("commandDialog_{}_{}", tab.title, i).c_str(), {-1, -1});
        {
          for (const auto& [code, needsElipses, override, forMV, forMZ] : column) {
            const auto label = (override.empty() ? DecodeEnumName(code) : override) + (needsElipses ? "..." : "");
            if ((forMV && Database::instance()->isMV()) || (forMZ && Database::instance()->isMZ())) {
              if (ImGui::Button(label.c_str(), size)) {
                m_commandDialog = CreateCommandDialog(code);
                if (m_commandDialog) {
                  m_commandDialog->setOpen(true);
                }
              }
            }
          }
        }
        ImGui::EndVertical();
        ++i;
      }
    }
    ImGui::EndHorizontal();
    ImGui::EndTabItem();
  }
}

std::list<EventCommandEditor::EventCommandTab> EventCommandEditor::buildTabList() {
  return {
      EventCommandTab{
          tr("Actor"),
          {
              {
                  {EventCode::Change_HP, true},
                  {EventCode::Change_MP, true},
                  {EventCode::Change_TP, true},
                  {EventCode::Change_EXP, true},
                  {EventCode::Change_Level, true},
                  {EventCode::Change_Skill, true},
              },
              {
                  {EventCode::Change_Gold, true},
                  {EventCode::Change_State, true},
                  {EventCode::Change_Equipment, true},
                  {EventCode::Change_Name, true},
                  {EventCode::Change_Class, true},
                  {EventCode::Recover_All, true},
              },
              {
                  {EventCode::Change_Items, true},
                  {EventCode::Change_Weapons, true},
                  {EventCode::Change_Armors, true},
                  {EventCode::Change_Party_Member, true},
                  {EventCode::Change_Nickname, true},
                  {EventCode::Change_Profile, true},
                  {EventCode::Change_Parameter, true},
              },
              {
                  {EventCode::Enemy_Recover_All, true},
                  {EventCode::Enemy_Appear, true},
                  {EventCode::Enemy_Transform, true},
                  {EventCode::Change_Enemy_HP, true},
                  {EventCode::Change_Enemy_MP, true},
                  {EventCode::Change_Enemy_TP, true},
                  {EventCode::Change_Enemy_State, true},
              },
          },
      },
      EventCommandTab{
          tr("Audio"),
          {
              {
                  {EventCode::Play_BGM, true},
                  {EventCode::Save_BGM, false},
                  {EventCode::Fadeout_BGM, true},
                  {EventCode::Resume_BGM, false, trNOOP("Resume BGM")},
              },
              {
                  {EventCode::Play_BGS, true},
                  {EventCode::Fadeout_BGS, true},
                  {EventCode::Play_Movie, true},
              },
              {
                  {EventCode::Play_ME, true},
                  {EventCode::Play_SE, true},
                  {EventCode::Stop_SE, false},
              },
          },
      },
      EventCommandTab{
          tr("Flow Control"),
          {
              {
                  {EventCode::Comment, true},
                  {EventCode::Conditional_Branch, true},
                  {EventCode::Loop, false},
                  {EventCode::Label, true},
                  {EventCode::Jump_To_Label, true},
                  {EventCode::Break_Loop, false},
                  {EventCode::Wait, true},
              },
              {
                  {EventCode::Control_Switches, true},
                  {EventCode::Control_Self_Switch, true},
                  {EventCode::Control_Variables, true},
                  {EventCode::Common_Event, true},
                  {EventCode::Control_Timer, true},
                  {EventCode::Erase_Event, false},
                  {EventCode::Exit_Event_Processing, false},
              },
              {
                  {EventCode::Transfer_Player, true},
                  {EventCode::Set_Event_Location, true},
                  {EventCode::Set_Vehicle_Location, true},
                  {EventCode::Script, true},
                  {EventCode::Plugin_Command_del_MV, true, {}, true, false},
                  {EventCode::Plugin_Command_del_MZ, true, {}, false, true},
                  {EventCode::Game_Over, false},
                  {EventCode::Return_To_Title_Screen, false},
              },
          },
      },
      EventCommandTab{
          tr("Scene"),
          {
              {
                  {EventCode::Show_Text, true},
                  {EventCode::Show_Choices, true},
                  {EventCode::Show_Scrolling_Text, true},
                  {EventCode::Input_Number, true},
                  {EventCode::Select_Item, true},
                  {EventCode::Set_Movement_Route, true},
              },
              {
                  {EventCode::Show_Animation, true},
                  {EventCode::Show_Battle_Animation, true},
                  {EventCode::Show_Balloon_Icon, true},
                  {EventCode::Shop_Processing, true},
                  {EventCode::Battle_Processing, true},
                  {EventCode::Name_Input_Processing, true},
              },
              {
                  {EventCode::Change_Transparency, true},
                  {EventCode::Get_on_fwsl_off_Vehicle},
                  {EventCode::Abort_Battle, false},
                  {EventCode::Gather_Followers, false},
              },
          },
      },
      EventCommandTab{
          trNOOP("Screen"),
          {
              {
                  {EventCode::Show_Picture, true},
                  {EventCode::Move_Picture, true},
                  {EventCode::Rotate_Picture, true},
                  {EventCode::Tint_Picture, true},
                  {EventCode::Erase_Picture, true},
              },
              {
                  {EventCode::Tint_Screen, true},
                  {EventCode::Flash_Screen, true},
                  {EventCode::Shake_Screen, true},
                  {EventCode::Fadeout_Screen, true},
                  {EventCode::Fadein_Screen, true},
              },
              {
                  {EventCode::Set_Weather_Effect, true},
                  {EventCode::Scroll_Map, true},
              },
          },
      },
      EventCommandTab{
          tr("System"),
          {
              {
                  {EventCode::Open_Menu_Screen, false},
                  {EventCode::Open_Save_Screen, false},
                  {EventCode::Change_Menu_Access, true},
                  {EventCode::Change_Save_Access, true},
                  {EventCode::Change_Formation_Access, true},
                  {EventCode::Change_Encounter, true},
              },
              {
                  {EventCode::Change_Battle_BGM, true},
                  {EventCode::Change_Vehicle_BGM, true},
                  {EventCode::Change_Victory_ME, true},
                  {EventCode::Change_Defeat_ME, true},
                  {EventCode::Change_Map_Display_Name, true},
                  {EventCode::Change_Parallax, true},
              },
              {
                  {EventCode::Change_Actor_Images, true},
                  {EventCode::Change_Tileset, true},
                  {EventCode::Change_Battle_Back, true},
                  {EventCode::Change_Vehicle_Image, true},
                  {EventCode::Change_Player_Followers, true},
                  {EventCode::Change_Window_Color, true},
              },
          },
      },
  };
}

float EventCommandEditor::findLargestButtonWidth(const std::list<EventCommandTab>& tabs) {
  float maxWidth = 0;

  for (const auto& [_, buttonColumns] : tabs) {
    for (const auto& column : buttonColumns) {
      for (const auto& button : column) {
        const auto label = (button.override.empty() ? DecodeEnumName(button.code) : button.override) + (button.needsElipses ? "..." : "");
        if (const auto width = ImGui::CalcTextSize(label.c_str()).x; width > maxWidth) {
          maxWidth = width;
        }
      }
    }
  }

  return maxWidth + (ImGui::GetStyle().ItemInnerSpacing.x * 2);
}

void EventCommandEditor::drawPopup() {
  if (m_selectedCommand < 0) {
    return;
  }
  if (!m_isRequested) {
    return;
  }

  const auto commandTabs = buildTabList();
  int numColumns = 0;
  int numButtons = 0;
  for (const auto& [_, buttonColumns] : commandTabs) {
    if (buttonColumns.size() > numColumns) {
      numColumns = buttonColumns.size();
      for (const auto& buttons : buttonColumns) {
        if (buttons.size() > numButtons) {
          numButtons = buttons.size();
        }
      }
    }
  }

  // numColumns++;
  const auto buttonWidth = findLargestButtonWidth(commandTabs);
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, {0.5f, 0.5f});
  ImGui::SetNextWindowSize(
      {buttonWidth * numColumns + (ImGui::GetStyle().FramePadding.x * (numColumns + 2)), ImGui::GetTextLineHeightWithSpacing() * (numButtons + 10) + (ImGui::GetStyle().FramePadding.y * 2)},
      ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(std::format("{}###command_picker", trNOOP("Command Window")).c_str(), nullptr,
                             ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
    bool close = false;
    ImGui::BeginVertical("##command_picker", ImGui::GetContentRegionAvail());
    {
      ImGui::BeginVertical("##command_picker_upper", {-1, -1});
      {
        // Event Templates
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::BeginCombo("##eventcommand_editor_presets", trNOOP("Select a preset to insert into selection..."))) {
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
                      command->adjustIndent(m_commands->at(m_selectedCommand)->indent().value());
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
                if (ImGui::Selectable(tr("Error loading template: " + templ.name()).c_str(), false)) {}
              }
            }
          }
          ImGui::EndCombo();
        }
        if (ImGui::BeginTabBar("##orpg_command_window")) {
          const auto size = ImVec2{buttonWidth, 0};

          for (int i = 0; const auto& tab : commandTabs) {
            drawCommandTab(tab, std::format("commandDialog_tab{}", i), size);
            ++i;
          }

          drawCommandDialog();
          ImGui::EndTabBar();
        }
      }
      ImGui::EndVertical();
      ImGui::Spring();
      ImGui::BeginVertical("##command_picker_lower", {-1, -1});
      {
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
        ImGui::BeginHorizontal("##buttons", {-1, -1});
        {
          ImGui::Spring();
          const int ret = ImGui::ButtonGroup("##action_buttons", {trNOOP("Cancel")});
          close = ret == 0;
        }
        ImGui::EndHorizontal();
      }
      ImGui::EndVertical();
    }
    ImGui::EndVertical();
    if (ImGui::IsKeyPressed(ImGuiKey_Escape) || close) {
      if (m_commandDialog) {
        m_commandDialog.reset();
      } else {
        m_isNewEntry = false;
        m_selectedCommand = 0;
        ImGui::CloseCurrentPopup();
      }
    }

    ImGui::EndPopup();
  } // End of "Command Window" Popup
}