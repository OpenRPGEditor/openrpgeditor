#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/Project.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiParsedText.hpp"
#include "Core/ImGuiUtils.hpp"

#include "imgui.h"
#include "imgui_internal.h"
#include "Core/Log.hpp"
#include "Core/EventCommands/Dialog_AbortBattle.hpp"
#include "Core/EventCommands/Dialog_BreakLoop.hpp"
#include "Core/EventCommands/Dialog_ChangeBattleBGM.hpp"
#include "Core/EventCommands/Dialog_ChangeClass.hpp"
#include "Core/EventCommands/Dialog_ChangeDefeatME.hpp"
#include "Core/EventCommands/Dialog_ChangeEncounter.hpp"
#include "Core/EventCommands/Dialog_ChangeEnemyState.hpp"
#include "Core/EventCommands/Dialog_ChangeEquipment.hpp"
#include "Core/EventCommands/Dialog_ChangeFormationAccess.hpp"
#include "Core/EventCommands/Dialog_ChangeGold.hpp"
#include "Core/EventCommands/Dialog_ChangeMenuAccess.hpp"
#include "Core/EventCommands/Dialog_ChangeName.hpp"
#include "Core/EventCommands/Dialog_ChangeNickname.hpp"
#include "Core/EventCommands/Dialog_ChangePartyMember.hpp"
#include "Core/EventCommands/Dialog_ChangeProfile.hpp"
#include "Core/EventCommands/Dialog_ChangeSaveAccess.hpp"
#include "Core/EventCommands/Dialog_ChangeVehicleBGM.hpp"
#include "Core/EventCommands/Dialog_ChangeVictoryME.hpp"
#include "Core/EventCommands/Dialog_ChangeWindowColor.hpp"
#include "Core/EventCommands/Dialog_CommonEvent.hpp"
#include "Core/EventCommands/Dialog_ControlSwitches.hpp"
#include "Core/EventCommands/Dialog_ControlVariables.hpp"
#include "Core/EventCommands/Dialog_Comment.hpp"
#include "Core/EventCommands/Dialog_EnemyAppear.hpp"
#include "Core/EventCommands/Dialog_EnemyRecoverAll.hpp"
#include "Core/EventCommands/Dialog_EnemyTransform.hpp"
#include "Core/EventCommands/Dialog_EraseEvent.hpp"
#include "Core/EventCommands/Dialog_ExitEventProcessing.hpp"
#include "Core/EventCommands/Dialog_GameOver.hpp"
#include "Core/EventCommands/Dialog_GatherFollowers.hpp"
#include "Core/EventCommands/Dialog_GetOnOffVehicle.hpp"
#include "Core/EventCommands/Dialog_JumpToLabel.hpp"
#include "Core/EventCommands/Dialog_Label.hpp"
#include "Core/EventCommands/Dialog_Loop.hpp"
#include "Core/EventCommands/Dialog_OpenMenuScreen.hpp"
#include "Core/EventCommands/Dialog_OpenSaveScreen.hpp"
#include "Core/EventCommands/Dialog_PlayBGM.hpp"
#include "Core/EventCommands/Dialog_PlayBGS.hpp"
#include "Core/EventCommands/Dialog_PlayME.hpp"
#include "Core/EventCommands/Dialog_PlayMovie.hpp"
#include "Core/EventCommands/Dialog_PlaySE.hpp"
#include "Core/EventCommands/Dialog_RecoverAll.hpp"
#include "Core/EventCommands/Dialog_ReplayBGM.hpp"
#include "Core/EventCommands/Dialog_ReturnToTitle.hpp"
#include "Core/EventCommands/Dialog_SaveBGM.hpp"
#include "Core/EventCommands/Dialog_Script.hpp"
#include "Core/EventCommands/Dialog_ShowAnimation.hpp"
#include "Core/EventCommands/Dialog_StopSE.hpp"
#include "Core/EventCommands/Dialog_TimerControl.hpp"
#include "Core/EventCommands/Dialog_Wait.hpp"
#include "Core/EventCommands/IDialogController.hpp"

#include "Database/EventCommands/BattleProcessing.hpp"
#include "Database/EventCommands/ChangeActorImage.hpp"
#include "Database/EventCommands/ChangeArmors.hpp"
#include "Database/EventCommands/ChangeClass.hpp"
#include "Database/EventCommands/ChangeEXP.hpp"
#include "Database/EventCommands/ChangeEnemyHP.hpp"
#include "Database/EventCommands/ChangeEnemyMP.hpp"
#include "Database/EventCommands/ChangeEnemyState.hpp"
#include "Database/EventCommands/ChangeEnemyTP.hpp"
#include "Database/EventCommands/ChangeEquipment.hpp"
#include "Database/EventCommands/ChangeHP.hpp"
#include "Database/EventCommands/ChangeItems.hpp"
#include "Database/EventCommands/ChangeLevel.hpp"
#include "Database/EventCommands/ChangeMP.hpp"
#include "Database/EventCommands/ChangeName.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/ChangeParameter.hpp"
#include "Database/EventCommands/ChangePartyMember.hpp"
#include "Database/EventCommands/ChangeProfile.hpp"
#include "Database/EventCommands/ChangeSkill.hpp"
#include "Database/EventCommands/ChangeState.hpp"
#include "Database/EventCommands/ChangeTP.hpp"
#include "Database/EventCommands/ChangeTileset.hpp"
#include "Database/EventCommands/ChangeWeapons.hpp"
#include "Database/EventCommands/CommonEvent.hpp"
#include "Database/EventCommands/ConditionalBranch.hpp"
#include "Database/EventCommands/EnemyAppear.hpp"
#include "Database/EventCommands/EnemyRecoverAll.hpp"
#include "Database/EventCommands/EnemyTransform.hpp"
#include "Database/EventCommands/ForceAction.hpp"
#include "Database/EventCommands/GetLocationInfo.hpp"
#include "Database/EventCommands/InputNumber.hpp"
#include "Database/EventCommands/NameInput.hpp"
#include "Database/EventCommands/RecoverAll.hpp"
#include "Database/EventCommands/Script.hpp"
#include "Database/EventCommands/SelectItem.hpp"
#include "Database/EventCommands/SetEventLocation.hpp"
#include "Database/EventCommands/SetMovementRoute.hpp"
#include "Database/EventCommands/SetVehicleLocation.hpp"
#include "Database/EventCommands/ShopProcessing.hpp"
#include "Database/EventCommands/ShowAnimation.hpp"
#include "Database/EventCommands/ShowBalloonIcon.hpp"
#include "Database/EventCommands/ShowBattleAnimation.hpp"
#include "Database/EventCommands/TransferPlayer.hpp"

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

      ImGui::TableSetupColumn("Selectable", ImGuiTableFlags_SizingFixedFit);
      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("Text");
      const int totalPadding = static_cast<int>(std::floor(std::log10(m_commands->size())));
      if (m_commands) {
        ImGui::PushFont(App::APP->getMonoFont());
        for (int n = 0; n < m_commands->size(); n++) {
          const bool isSelected = (m_selectedCommand == n);
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
                m_selectedCommand = n;
                m_isNewEntry = false;
                ImGui::OpenPopup("Command Window");
              }
              m_selectedCommand = n;
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
      drawPopup();
      ImGui::EndTable();
    }
  }
  ImGui::EndGroup();
}

void EventCommandEditor::drawPopup() {
  if (m_selectedCommand < 0) {
    return;
  }

  ImGui::SetNextWindowSize(ImVec2{680, 550} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal("Command Window", nullptr,
                             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
    if (!m_isNewEntry) {
      // We're not a new entry so copy our values so we can restore them if we cancel
      // m_tempTrait = s*m_selectedTrait;
    }
    ImGui::BeginGroup();
    {

      if (ImGui::BeginTabBar("##orpg_command_window")) {

        ImVec2 size = ImVec2{((ImGui::GetContentRegionAvail().x / 2) / 2) - App::DPIHandler::scale_value(15), 0};

        if (ImGui::BeginTabItem("Actor")) {
          if (ImGui::Button("Change HP...", size)) {}
          ImGui::SameLine();
          if (ImGui::Button("Change Gold...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeGold>("Change Gold", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy HP...", size)) {}
          if (ImGui::Button("Change MP...", size)) {}
          ImGui::SameLine();
          if (ImGui::Button("Change Items...", size)) {}
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy MP...", size)) {}
          if (ImGui::Button("Change TP...", size)) {}
          ImGui::SameLine();
          if (ImGui::Button("Change Weapons...", size)) {}
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy TP...", size)) {}
          if (ImGui::Button("Change State...", size)) {}
          ImGui::SameLine();
          if (ImGui::Button("Change Armors...", size)) {}
          ImGui::SameLine();
          if (ImGui::Button("Change Enemy State...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeEnemyState>("Change Enemy State", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Recover All...", size)) {
            commandDialog = std::make_shared<Dialog_RecoverAll>("Recover All", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Change Party Member...", size)) {
            commandDialog = std::make_shared<Dialog_ChangePartyMember>("Change Party Member", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Enemy Recover All...", size)) {
            commandDialog = std::make_shared<Dialog_EnemyRecoverAll>("Enemy Recover All", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change EXP...", size)) {}
          ImGui::SameLine();
          if (ImGui::Button("Change Nickname...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeNickname>("Change Nickname", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Enemy Appear...", size)) {
            commandDialog = std::make_shared<Dialog_EnemyAppear>("Enemy Appear", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Level...", size)) {}
          ImGui::SameLine();
          if (ImGui::Button("Change Profile...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeProfile>("Change Profile", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine();
          if (ImGui::Button("Enemy Transform...", size)) {
            commandDialog = std::make_shared<Dialog_EnemyTransform>("Enemy Transform", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Parameter...", size)) {}
          if (ImGui::Button("Change Skill...", size)) {}
          if (ImGui::Button("Change Equipment...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeEquipment>("Change Equipment", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Name...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeName>("Change Name", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Class...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeClass>("Change Class", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Audio")) {
          if (ImGui::Button("Play BGM...", size)) {
            commandDialog = std::make_shared<Dialog_PlayBGM>("Play BGM", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Play BGS...", size)) {
            commandDialog = std::make_shared<Dialog_PlayBGS>("Play BGS", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Play SE...", size)) {
            commandDialog = std::make_shared<Dialog_PlaySE>("Play SE", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Play ME...", size)) {
            commandDialog = std::make_shared<Dialog_PlayME>("Play ME", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Play Movie...", size)) {
            commandDialog = std::make_shared<Dialog_PlayMovie>("Play Movie", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Save BGM", size)) {
            commandDialog = std::make_shared<Dialog_SaveBGM>("Save BGM", m_project);
          }
          if (ImGui::Button("Replay BGM", size)) {
            commandDialog = std::make_shared<Dialog_ReplayBGM>("Replay BGM", m_project);
          }
          if (ImGui::Button("Stop SE", size)) {
            commandDialog = std::make_shared<Dialog_StopSE>("Stop SE", m_project);
          }
          if (ImGui::Button("Fadeout BGM...", size)) {}
          if (ImGui::Button("Fadeout BGS...", size)) {}
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Flow")) {
          if (ImGui::Button("Conditional Branch...", size)) {}
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Switches...", size)) {
            commandDialog = std::make_shared<Dialog_ControlSwitches>("Control Switches", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Wait...", size)) {
            commandDialog = std::make_shared<Dialog_Wait>("Wait", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Loop", size)) {
            commandDialog = std::make_shared<Dialog_Loop>("Loop", m_project);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Variables...", size)) {
            commandDialog = std::make_shared<Dialog_ControlVariables>("Control Variables", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Erase Event", size)) {
            commandDialog = std::make_shared<Dialog_EraseEvent>("Erase Event", m_project);
          }
          if (ImGui::Button("Break Loop", size)) {
            commandDialog = std::make_shared<Dialog_BreakLoop>("Break Loop", m_project);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Self Switch...", size)) {}
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Exit Event Processing", size)) {
            commandDialog = std::make_shared<Dialog_ExitEventProcessing>("Exit Event Processing", m_project);
          }
          if (ImGui::Button("Common Event...", size)) {
            commandDialog = std::make_shared<Dialog_CommonEvent>("Common Event", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Control Timer...", size)) {
            commandDialog = std::make_shared<Dialog_TimerControl>("Control Timer", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Label...", size)) {
            commandDialog = std::make_shared<Dialog_Label>("Label", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Transfer Player...", size)) {}
          if (ImGui::Button("Jump to Label...", size)) {
            commandDialog = std::make_shared<Dialog_JumpToLabel>("Jump to Label", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Set Event Location...", size)) {}
          if (ImGui::Button("Comment...", size)) {
            commandDialog = std::make_shared<Dialog_Comment>("Comment", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Set Vehicle Location...", size)) {}
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Scene")) {
          if (ImGui::Button("Show Text...", size)) {}
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Show Animation...", size)) {
            commandDialog = std::make_shared<Dialog_ShowAnimation>("Show Animation", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Change Transparency...", size)) {}
          if (ImGui::Button("Show Choices...", size)) {}
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Show Battle Animation...", size)) {
            // TODO
            // commandDialog = std::make_shared<Dialog_ShowBattleAnimation>("Show Battle Animation", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Get on/off Vehicle", size)) {
            commandDialog = std::make_shared<Dialog_GetOnOffVehicle>("Get on/off Vehicle", m_project);
          }
          if (ImGui::Button("Show Scrolling Text...", size)) {}
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Show Balloon Icon...", size)) {
            // TODO
            // commandDialog = std::make_shared<Dialog_ShowBalloonIcon>("Show Balloon Icon", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Abort Battle", size)) {
            commandDialog = std::make_shared<Dialog_AbortBattle>("Abort Battle", m_project);
          }
          if (ImGui::Button("Input Number...", size)) {
            // TODO
            // commandDialog = std::make_shared<Dialog_InputNumber>("Input Number", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Shop Processing...", size)) {}
          ImGui::SameLine(); // Third Column
          if (ImGui::Button("Gather Followers", size)) {
            commandDialog = std::make_shared<Dialog_GatherFollowers>("Gather Followers", m_project);
          }
          if (ImGui::Button("Select Item...", size)) {
            // TODO
            // commandDialog = std::make_shared<Dialog_SelectItem>("Select Item", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Battle Processing...", size)) {}
          if (ImGui::Button("Set Movement Route...", size)) {}
          ImGui::SameLine(); // Second Column
          if (ImGui::Button("Name Input Processing...", size)) {
            // TODO
            // commandDialog = std::make_shared<Dialog_NameInputProcessing>("Name Input Processing", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Screen")) {
          if (ImGui::Button("Show Picture...", size)) {}
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Tint Screen...", size)) {}
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Set Weather Effect...", size)) {}
          if (ImGui::Button("Move Picture...", size)) {}
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Flash Screen...", size)) {}
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Scroll Map...", size)) {
            // TODO
            // commandDialog = std::make_shared<Dialog_ScrollMap>("Scroll Map", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Rotate Picture...", size)) {
            // TODO
            // commandDialog = std::make_shared<Dialog_RotatePicture>("Rotate Picture", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Shake Screen...", size)) {}
          if (ImGui::Button("Tint Picture...", size)) {}
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Fadeout Screen...", size)) {}
          if (ImGui::Button("Erase Picture...", size)) {
            // TODO
            // commandDialog = std::make_shared<Dialog_ErasePicture>("Erase Picture", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Fadein Screen...", size)) {}
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("System")) {
          if (ImGui::Button("Open Menu Screen", size)) {
            commandDialog = std::make_shared<Dialog_OpenMenuScreen>("Open Menu Screen", m_project);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Battle BGM...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeBattleBGM>("Change Battle BGM", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Script...", size)) {
            commandDialog = std::make_shared<Dialog_Script>("Script", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Open Save Screen", size)) {
            commandDialog = std::make_shared<Dialog_OpenSaveScreen>("Save Screen", m_project);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Vehicle BGM...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeVehicleBGM>("Change Vehicle BGM", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Plugin Command...", size)) {}
          if (ImGui::Button("Change Menu Access...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeMenuAccess>("Change Menu Access", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Victory ME...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeVictoryME>("Change Victory ME", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Game Over", size)) {
            commandDialog = std::make_shared<Dialog_GameOver>("Game Over", m_project);
          }
          if (ImGui::Button("Change Save Access...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeSaveAccess>("Change Save Access", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Defeat ME...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeDefeatME>("Change Defeat ME", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Third column
          if (ImGui::Button("Return To Title Screen", size)) {
            commandDialog = std::make_shared<Dialog_ReturnToTitle>("Return To Title Screen", m_project);
          }
          if (ImGui::Button("Change Map Display Name...", size)) {}
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Parallax...", size)) {}
          if (ImGui::Button("Change Tileset...", size)) {
            // TODO
            // commandDialog = std::make_shared<Dialog_ChangeTileset>("Change Tileset", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Actor Images...", size)) {}
          if (ImGui::Button("Change Battle Back...", size)) {
            // TODO : We need a specific image picker for this one (double images w/ preview)
            // commandDialog = std::make_shared<Dialog_ChangeBattleBack>("Change Battle Back", m_project);
            // commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Vehicle Image...", size)) {}
          if (ImGui::Button("Change Player Followers...", size)) {}
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Window Color...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeWindowColor>("Change Window Color", m_project);
            commandDialog->SetOpen(true);
          }
          if (ImGui::Button("Change Formation Access...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeFormationAccess>("Change Formation Access", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::SameLine(); // Second column
          if (ImGui::Button("Change Encounter...", size)) {
            commandDialog = std::make_shared<Dialog_ChangeEncounter>("Change Encounter", m_project);
            commandDialog->SetOpen(true);
          }
          ImGui::EndTabItem();
        }

        if (commandDialog) {
          auto [closed, confirmed] = commandDialog->draw();
          if (confirmed) {
            auto m_select = m_commands->begin() + m_selectedCommand;
            m_select = m_commands->insert(m_select, std::make_shared<IEventCommand>(commandDialog->getCommand()));
            commandDialog.reset();
            ImGui::CloseCurrentPopup();
          }
        }

        ImGui::EndTabBar();
      }
    }
    ImGui::EndGroup();

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
      m_isNewEntry = false;
      m_selectedCommand = 0;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  } // End of "Command Window" Popup
}