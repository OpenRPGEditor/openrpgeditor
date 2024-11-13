#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/ImGuiExt/ImGuiParsedText.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/Application.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "Core/Log.hpp"
#include "Database/EventCommands/RepeatAbove.hpp"

#include <iostream>
#include <vector>

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
    if (ImGui::BeginTable("##commonevent_code_contents", 2,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX |
                              ImGuiTableFlags_ScrollY,
                          ImVec2{0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(16)})) {
      ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.37f, 0.37f, 0.37f, 0.43f));
      ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.37f, 0.37f, 0.37f, 0.78f));
      ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.37f, 0.37f, 0.37f, 0.68f));
      ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(0.03f, 0.21f, 0.26f, 1.00f));
      ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(0.04f, 0.16f, 0.19f, 1.00f));

      ImGui::TableSetupColumn("Selectable", ImGuiTableFlags_SizingFixedFit);
      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("Text");
      const int totalPadding = static_cast<int>(std::floor(std::log10(m_commands->size())));
      if (m_commands) {
        ImGui::PushFont(App::APP->getMonoFont());
        for (int n = 0; n < m_commands->size(); n++) {
          const bool isSelected =
              (m_selectedCommand == n || (m_selectedEnd != -1 && n > m_selectedCommand && n <= m_selectedEnd));
          std::string indentPad = m_commands->at(n)->stringRep(m_project->database());
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
            if (ImGui::SelectableWithBorder((std::string(stepPadding, ' ') + std::to_string(step) + " ").c_str(),
                                            isSelected,
                                            ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns |
                                                ImGuiSelectableFlags_AllowDoubleClick,
                                            ImVec2(0, height))) {
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                if (m_commands->at(n)->code() == EventCode::Event_Dummy) {
                  m_isNewEntry = true;
                  ImGui::OpenPopup("Command Window");
                } else {
                  m_isNewEntry = false;
                  /* These are generated automatically based on their related event command dialogs */
                  if (m_commands->at(n)->code() != EventCode::Repeat_Above &&
                      m_commands->at(n)->code() != EventCode::Else && m_commands->at(n)->code() != EventCode::End) {
                    commandDialog = CreateCommandDialog(m_commands->at(n)->code(), m_commands->at(n));
                    commandDialog->SetOpen(true);
                    // ImGui::OpenPopup("Command Window");
                  }
                }
              }
              m_selectedCommand = n;
              if (m_commands->at(n)->code() == EventCode::Loop) {
                int j = n + 1;
                while (true) {
                  if ((m_commands->at(j)->code() == EventCode::Repeat_Above &&
                       *m_commands->at(j)->indent == *m_commands->at(n)->indent) ||
                      j >= m_commands->size()) {
                    break;
                  }
                  ++j;
                }
                m_selectedEnd = j;
              } else if (m_commands->at(n)->code() == EventCode::Repeat_Above) {
                m_selectedEnd = n;
                int j = n - 1;
                while (true) {
                  if ((m_commands->at(j)->code() == EventCode::Loop &&
                       *m_commands->at(j)->indent == *m_commands->at(n)->indent) ||
                      j < 0) {
                    break;
                  }
                  --j;
                }
                m_selectedCommand = j;
              } else {
                m_selectedEnd = -1;
              }
            }
            if (ImGui::IsItemHovered()) {
              m_hoveringCommand = n;
            }
          }
          if (ImGui::TableNextColumn()) {
            for (const auto& s : str) {
              ImGui::TextParsed("&push-color=0xFF,0xFF,0xFF;%s&pop-color;", s.c_str());
            }
          }

          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::PopFont();
      }
      ImGui::PopStyleColor(5);

      if (ImGui::BeginPopupContextWindow()) {
        if (m_selectedCommand != m_hoveringCommand) {
          m_selectedCommand = m_hoveringCommand;
        }
        if (ImGui::Button("New...", ImVec2(200.0f, 0.0f))) {
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
  }
  ImGui::EndGroup();
}

void EventCommandEditor::drawCommandDialog() {
  if (commandDialog) {
    auto [closed, confirmed] = commandDialog->draw();
    if (!commandDialog->getParentIndent().has_value()) {
      commandDialog->setParentIndent(m_commands->at(m_selectedCommand)->indent.value());
      commandDialog->getCommand()->indent = commandDialog->getParentIndent().value();
    }

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
          std::shared_ptr<ShowChoiceCommand> commandPointer =
              std::dynamic_pointer_cast<ShowChoiceCommand>(commandDialog->getCommand());

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
                std::shared_ptr<WhenSelectedCommand> when =
                    std::static_pointer_cast<WhenSelectedCommand>(m_commands->at(i));

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
      commandDialog.reset();
      ImGui::CloseCurrentPopup();
    }
  }
}
void EventCommandEditor::drawPopup() {
  if (m_selectedCommand < 0) {
    return;
  }

  ImGui::SetNextWindowSize(ImVec2{680, 550} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal("Command Window", nullptr,
                             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
    ImGui::BeginGroup();
    {

      if (ImGui::BeginTabBar("##orpg_command_window")) {

        ImVec2 size = ImVec2{((ImGui::GetContentRegionAvail().x / 2) / 2) + App::DPIHandler::scale_value(50), 0};

        if (ImGui::BeginTabItem("Actor")) {
          if (ImGui::Button("Change HP...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_HP);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Gold...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Gold);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy HP...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Enemy_HP);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change MP...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_MP);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Items...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Items);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy MP...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Enemy_MP);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change TP...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_TP);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Weapons...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Weapons);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy TP...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Enemy_TP);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change State...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_State);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Armors...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Armors);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy State...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Enemy_State);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Recover All...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Recover_All);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Party Member...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Party_Member);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Enemy Recover All...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Enemy_Recover_All);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change EXP...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_EXP);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Nickname...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Nickname);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Enemy Appear...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Enemy_Appear);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Level...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Level);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Profile...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Profile);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Enemy Transform...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Enemy_Transform);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Parameter...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Parameter);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Skill...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Skill);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Equipment...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Equipment);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Name...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Name);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Class...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Class);
            commandDialog->SetOpen(true);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Audio")) {
          ImGui::BeginGroup();
          {
            if (ImGui::Button("Play BGM...", size)) {
              commandDialog = CreateCommandDialog(EventCode::Play_BGM);
              commandDialog->SetOpen(true);
            }
            if (ImGui::Button("Save BGM", size)) {
              commandDialog = CreateCommandDialog(EventCode::Save_BGM);
            }
            if (ImGui::Button("Fadeout BGM...", size)) {
              commandDialog = CreateCommandDialog(EventCode::Fadeout_BGM);
              commandDialog->SetOpen(true);
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
              commandDialog->SetOpen(true);
            }
            if (ImGui::Button("Fadeout BGS...", size)) {
              commandDialog = CreateCommandDialog(EventCode::Fadeout_BGS);
              commandDialog->SetOpen(true);
            }
            if (ImGui::Button("Play Movie...", size)) {
              commandDialog = CreateCommandDialog(EventCode::Play_Movie);
              commandDialog->SetOpen(true);
            }
          }
          ImGui::EndGroup();
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            if (ImGui::Button("Play ME...", size)) {
              commandDialog = CreateCommandDialog(EventCode::Play_ME);
              commandDialog->SetOpen(true);
            }
            if (ImGui::Button("Play SE...", size)) {
              commandDialog = CreateCommandDialog(EventCode::Play_SE);
              commandDialog->SetOpen(true);
            }
            if (ImGui::Button("Stop SE", size)) {
              commandDialog = CreateCommandDialog(EventCode::Stop_SE);
            }
          }
          ImGui::EndGroup();
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Flow")) {
          if (ImGui::Button("Conditional Branch...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Conditional_Branch);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Switches...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Control_Switches);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Wait...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Wait);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Loop", size)) {
            commandDialog = CreateCommandDialog(EventCode::Loop);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Variables...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Control_Variables);
            commandDialog->SetOpen(true);
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
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Exit Event Processing", size)) {
            commandDialog = CreateCommandDialog(EventCode::Exit_Event_Processing);
          }
          if (ImGui::Button("Common Event...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Common_Event);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Timer...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Control_Timer);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Label...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Label);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Transfer Player...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Transfer_Player);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Jump to Label...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Jump_To_Label);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Set Event Location...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Set_Event_Location);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Comment...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Comment);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Set Vehicle Location...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Set_Vehicle_Location);
            commandDialog->SetOpen(true);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Scene")) {
          if (ImGui::Button("Show Text...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Show_Text);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Show Animation...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Show_Animation);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Change Transparency...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Transparency);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Show Choices...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Show_Choices);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Show Battle Animation...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Show_Battle_Animation);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Get on/off Vehicle", size)) {
            commandDialog = CreateCommandDialog(EventCode::Get_On_Off_Vehicle);
          }
          if (ImGui::Button("Show Scrolling Text...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Show_Scrolling_Text);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Show Balloon Icon...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Show_Balloon_Icon);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Abort Battle", size)) {
            commandDialog = CreateCommandDialog(EventCode::Abort_Battle);
          }
          if (ImGui::Button("Input Number...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Input_Number);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Shop Processing...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Shop_Processing);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Gather Followers", size)) {
            commandDialog = CreateCommandDialog(EventCode::Gather_Followers);
          }
          if (ImGui::Button("Select Item...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Select_Item);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Battle Processing...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Battle_Processing);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Set Movement Route...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Set_Movement_Route);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Name Input Processing...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Name_Input_Processing);
            commandDialog->SetOpen(true);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Screen")) {
          if (ImGui::Button("Show Picture...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Show_Picture);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Tint Screen...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Tint_Screen);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Set Weather Effect...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Set_Weather_Effect);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Move Picture...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Move_Picture);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Flash Screen...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Flash_Screen);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Scroll Map...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Scroll_Map);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Rotate Picture...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Rotate_Picture);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Shake Screen...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Shake_Screen);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Tint Picture...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Tint_Picture);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Fadeout Screen...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Fadeout_Screen);
          }
          if (ImGui::Button("Erase Picture...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Erase_Picture);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Fadein Screen...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Fadein_Screen);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("System")) {
          if (ImGui::Button("Open Menu Screen", size)) {
            commandDialog = CreateCommandDialog(EventCode::Open_Menu_Screen);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Battle BGM...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Battle_BGM);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Script...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Script);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Open Save Screen", size)) {
            commandDialog = CreateCommandDialog(EventCode::Open_Save_Screen);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Vehicle BGM...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Vechicle_BGM);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Plugin Command...", size)) {
            commandDialog = CreateCommandDialog(EventCode::PluginMV_Command);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Menu Access...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Menu_Access);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Victory ME...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Victory_ME);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Game Over", size)) {
            commandDialog = CreateCommandDialog(EventCode::Game_Over);
          }
          if (ImGui::Button("Change Save Access...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Save_Access);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Defeat ME...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Defeat_ME);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Return To Title Screen", size)) {
            commandDialog = CreateCommandDialog(EventCode::Return_To_Title_Screen);
          }
          if (ImGui::Button("Change Map Display Name...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Map_Name_Display);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Parallax...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Parallax);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Tileset...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Tileset);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Actor Images...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Actor_Images);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Battle Back...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Battle_Back);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Vehicle Image...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Vehicle_Image);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Player Followers...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Player_Followers);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Window Color...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Window_Color);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Formation Access...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Formation_Access);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Encounter...", size)) {
            commandDialog = CreateCommandDialog(EventCode::Change_Encounter_Disable);
            commandDialog->SetOpen(true);
          }
          ImGui::EndTabItem();
        }

        // Event Templates
        ImGui::PushItemWidth(App::DPIHandler::scale_value(500));
        if (ImGui::BeginCombo("##commandwindow_templates", m_currentTemplate.c_str())) {
          for (const auto self : {"Window Display", "Window Reset", "ARPG - Start", "ARPG - Stop"}) {
            if (ImGui::Selectable(self, self == m_currentTemplate)) {
              m_currentTemplate = self;
            }
          }
          ImGui::EndCombo();
        }

        drawCommandDialog();

        ImGui::EndTabBar();
      }
    }
    ImGui::EndGroup();

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
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
