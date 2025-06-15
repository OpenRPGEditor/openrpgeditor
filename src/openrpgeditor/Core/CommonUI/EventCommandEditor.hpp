#pragma once

#include "Core/EventCommands/Dialog_AbortBattle.hpp"
#include "Core/EventCommands/Dialog_BattleProcessing.hpp"
#include "Core/EventCommands/Dialog_BreakLoop.hpp"
#include "Core/EventCommands/Dialog_ChangeActorImages.hpp"
#include "Core/EventCommands/Dialog_ChangeArmors.hpp"
#include "Core/EventCommands/Dialog_ChangeBattleBack.hpp"
#include "Core/EventCommands/Dialog_ChangeBattleBGM.hpp"
#include "Core/EventCommands/Dialog_ChangeClass.hpp"
#include "Core/EventCommands/Dialog_ChangeDefeatME.hpp"
#include "Core/EventCommands/Dialog_ChangeEncounter.hpp"
#include "Core/EventCommands/Dialog_ChangeEnemyHP.hpp"
#include "Core/EventCommands/Dialog_ChangeEnemyMP.hpp"
#include "Core/EventCommands/Dialog_ChangeEnemyState.hpp"
#include "Core/EventCommands/Dialog_ChangeEnemyTP.hpp"
#include "Core/EventCommands/Dialog_ChangeEquipment.hpp"
#include "Core/EventCommands/Dialog_ChangeEXP.hpp"
#include "Core/EventCommands/Dialog_ChangeFormationAccess.hpp"
#include "Core/EventCommands/Dialog_ChangeGold.hpp"
#include "Core/EventCommands/Dialog_ChangeHP.hpp"
#include "Core/EventCommands/Dialog_ChangeItems.hpp"
#include "Core/EventCommands/Dialog_ChangeLevel.hpp"
#include "Core/EventCommands/Dialog_ChangeMapDisplayName.hpp"
#include "Core/EventCommands/Dialog_ChangeMenuAccess.hpp"
#include "Core/EventCommands/Dialog_ChangeMP.hpp"
#include "Core/EventCommands/Dialog_ChangeName.hpp"
#include "Core/EventCommands/Dialog_ChangeNickname.hpp"
#include "Core/EventCommands/Dialog_ChangeParallax.hpp"
#include "Core/EventCommands/Dialog_ChangeParameter.hpp"
#include "Core/EventCommands/Dialog_ChangePartyMember.hpp"
#include "Core/EventCommands/Dialog_ChangePlayerFollowers.hpp"
#include "Core/EventCommands/Dialog_ChangePluginCommand.hpp"
#include "Core/EventCommands/Dialog_ChangeProfile.hpp"
#include "Core/EventCommands/Dialog_ChangeSaveAccess.hpp"
#include "Core/EventCommands/Dialog_ChangeSkill.hpp"
#include "Core/EventCommands/Dialog_ChangeState.hpp"
#include "Core/EventCommands/Dialog_ChangeTileset.hpp"
#include "Core/EventCommands/Dialog_ChangeTP.hpp"
#include "Core/EventCommands/Dialog_ChangeTransparency.hpp"
#include "Core/EventCommands/Dialog_ChangeVehicleBGM.hpp"
#include "Core/EventCommands/Dialog_ChangeVehicleImage.hpp"
#include "Core/EventCommands/Dialog_ChangeVictoryME.hpp"
#include "Core/EventCommands/Dialog_ChangeWeapons.hpp"
#include "Core/EventCommands/Dialog_ChangeWindowColor.hpp"
#include "Core/EventCommands/Dialog_Comment.hpp"
#include "Core/EventCommands/Dialog_CommonEvent.hpp"
#include "Core/EventCommands/Dialog_ConditionalBranch.hpp"
#include "Core/EventCommands/Dialog_ControlSelfSwitch.hpp"
#include "Core/EventCommands/Dialog_ControlSwitches.hpp"
#include "Core/EventCommands/Dialog_ControlTimer.hpp"
#include "Core/EventCommands/Dialog_ControlVariables.hpp"
#include "Core/EventCommands/Dialog_EnemyAppear.hpp"
#include "Core/EventCommands/Dialog_EnemyRecoverAll.hpp"
#include "Core/EventCommands/Dialog_EnemyTransform.hpp"
#include "Core/EventCommands/Dialog_EraseEvent.hpp"
#include "Core/EventCommands/Dialog_ErasePicture.hpp"
#include "Core/EventCommands/Dialog_ExitEventProcessing.hpp"
#include "Core/EventCommands/Dialog_FadeinScreen.hpp"
#include "Core/EventCommands/Dialog_FadeoutBGM.hpp"
#include "Core/EventCommands/Dialog_FadeoutBGS.hpp"
#include "Core/EventCommands/Dialog_FadeoutScreen.hpp"
#include "Core/EventCommands/Dialog_FlashScreen.hpp"
#include "Core/EventCommands/Dialog_GameOver.hpp"
#include "Core/EventCommands/Dialog_GatherFollowers.hpp"
#include "Core/EventCommands/Dialog_GetOnOffVehicle.hpp"
#include "Core/EventCommands/Dialog_InputNumber.hpp"
#include "Core/EventCommands/Dialog_JumpToLabel.hpp"
#include "Core/EventCommands/Dialog_Label.hpp"
#include "Core/EventCommands/Dialog_Loop.hpp"
#include "Core/EventCommands/Dialog_MovePicture.hpp"
#include "Core/EventCommands/Dialog_NameInputProcessing.hpp"
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
#include "Core/EventCommands/Dialog_RotatePicture.hpp"
#include "Core/EventCommands/Dialog_SaveBGM.hpp"
#include "Core/EventCommands/Dialog_Script.hpp"
#include "Core/EventCommands/Dialog_ScrollMap.hpp"
#include "Core/EventCommands/Dialog_SelectItem.hpp"
#include "Core/EventCommands/Dialog_SetEventLocation.hpp"
#include "Core/EventCommands/Dialog_SetMovementRoute.hpp"
#include "Core/EventCommands/Dialog_SetVehicleLocation.hpp"
#include "Core/EventCommands/Dialog_SetWeatherEffect.hpp"
#include "Core/EventCommands/Dialog_ShakeScreen.hpp"
#include "Core/EventCommands/Dialog_ShopProcessing.hpp"
#include "Core/EventCommands/Dialog_ShowAnimation.hpp"
#include "Core/EventCommands/Dialog_ShowBalloonIcon.hpp"
#include "Core/EventCommands/Dialog_ShowBattleAnimation.hpp"
#include "Core/EventCommands/Dialog_ShowChoice.hpp"
#include "Core/EventCommands/Dialog_ShowPicture.hpp"
#include "Core/EventCommands/Dialog_ShowScrollingText.hpp"
#include "Core/EventCommands/Dialog_ShowText.hpp"
#include "Core/EventCommands/Dialog_StopSE.hpp"
#include "Core/EventCommands/Dialog_TintPicture.hpp"
#include "Core/EventCommands/Dialog_TintScreen.hpp"
#include "Core/EventCommands/Dialog_TransferPlayer.hpp"
#include "Core/EventCommands/Dialog_Wait.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/BattleProcessing.hpp"
#include "Database/EventCommands/ChangeActorImage.hpp"
#include "Database/EventCommands/ChangeArmors.hpp"
#include "Database/EventCommands/ChangeClass.hpp"
#include "Database/EventCommands/ChangeEnemyHP.hpp"
#include "Database/EventCommands/ChangeEnemyMP.hpp"
#include "Database/EventCommands/ChangeEnemyState.hpp"
#include "Database/EventCommands/ChangeEnemyTP.hpp"
#include "Database/EventCommands/ChangeEquipment.hpp"
#include "Database/EventCommands/ChangeEXP.hpp"
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
#include "Database/EventCommands/ChangeTileset.hpp"
#include "Database/EventCommands/ChangeTP.hpp"
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

#include <list>
#include <memory>
#include <vector>

struct IEventCommand;
struct MainWindow;
struct EventCommandEditor {
  struct EventCommandButton {
    EventCode code;
    bool needsElipses;
    std::string override;
    bool forMV = true;
    bool forMZ = true;
  };

  struct EventCommandTab {
    std::string title;
    std::list<std::list<EventCommandButton>> buttonColumns;
  };

  EventCommandEditor()
  : m_commandDialog(nullptr) {}
  void blockSelect(int n, bool isDelete);
  void handleClipboardInteraction();
  static void setupTableHeader();
  static void setupTableColors();
  void handleBlockCollapse(int& n) const;
  void draw();

  void setCommands(std::vector<std::shared_ptr<IEventCommand>>* commands) {
    m_commands = commands;
    m_selectedCommand = -1;
  }

  void setTroopId(int troopId) { m_troopId = troopId; }

private:
  static std::list<EventCommandTab> buildTabList();
  static float findLargestButtonWidth(const std::list<EventCommandTab>& tabs);
  void drawPopup();
  void drawCommandDialog();
  void drawCommandTab(const EventCommandTab& tab, const std::string&, ImVec2 size);

  bool m_isNewEntry{false};
  bool m_needsUpdate{false};
  bool m_isRequested{false};
  int m_currentTemplate{-1};
  int m_selectedCommand = {-1};
  int m_selectedEnd = -1;
  int m_hoveringCommand = -1;
  int m_hoveringEnd = -1;
  bool m_hasFocus{false};
  int m_troopId = 0;

  std::shared_ptr<IEventDialogController> m_commandDialog;
  std::vector<std::shared_ptr<IEventCommand>>* m_commands = nullptr;

  std::shared_ptr<IEventDialogController> CreateCommandDialog(EventCode code, std::shared_ptr<IEventCommand> cmd = nullptr) {
    switch (code) {
    case EventCode::Battle_Processing:
      return std::make_shared<Dialog_BattleProcessing>(DecodeEnumName(code), std::dynamic_pointer_cast<BattleProcessingCommand>(cmd));
    case EventCode::Event_Dummy:
      break;
    case EventCode::Show_Text:
      return std::make_shared<Dialog_ShowText>(DecodeEnumName(code), std::dynamic_pointer_cast<ShowTextCommand>(cmd));
    case EventCode::Next_Text:
      break;
    case EventCode::Show_Choices:
      return std::make_shared<Dialog_ShowChoice>(DecodeEnumName(code), std::dynamic_pointer_cast<ShowChoiceCommand>(cmd));
    case EventCode::When_Selected:
      break;
    case EventCode::When_Cancel:
      break;
    case EventCode::End_del_ShowChoices:
      break;
    case EventCode::Input_Number:
      return std::make_shared<Dialog_InputNumber>(DecodeEnumName(code), std::dynamic_pointer_cast<InputNumberCommand>(cmd));
    case EventCode::Select_Item:
      return std::make_shared<Dialog_SelectItem>(DecodeEnumName(code), std::dynamic_pointer_cast<SelectItemCommand>(cmd));
    case EventCode::Show_Scrolling_Text:
      return std::make_shared<Dialog_ShowScrollingText>(DecodeEnumName(code), std::dynamic_pointer_cast<ShowScrollTextCommand>(cmd));
    case EventCode::Next_Scrolling_Text:
      break;
    case EventCode::Comment:
      return std::make_shared<Dialog_Comment>(DecodeEnumName(code), std::dynamic_pointer_cast<CommentCommand>(cmd));
    case EventCode::Next_Comment:
      break;
    case EventCode::Conditional_Branch:
      return std::make_shared<Dialog_ConditionalBranch>(DecodeEnumName(code), std::dynamic_pointer_cast<ConditionalBranchCommand>(cmd), m_troopId);
    case EventCode::Else:
      break;
    case EventCode::End:
      break;
    case EventCode::Loop:
      return std::make_shared<Dialog_Loop>(DecodeEnumName(code), std::dynamic_pointer_cast<LoopCommand>(cmd));
    case EventCode::Repeat_Above:
      break;
    case EventCode::Break_Loop:
      return std::make_shared<Dialog_BreakLoop>(DecodeEnumName(code), std::dynamic_pointer_cast<BreakLoopCommand>(cmd));
    case EventCode::Exit_Event_Processing:
      return std::make_shared<Dialog_ExitEventProcessing>(DecodeEnumName(code), std::dynamic_pointer_cast<ExitEventProcessingCommand>(cmd));
    case EventCode::Common_Event:
      return std::make_shared<Dialog_CommonEvent>(DecodeEnumName(code), std::dynamic_pointer_cast<CommonEventCommand>(cmd));
    case EventCode::Label:
      return std::make_shared<Dialog_Label>(DecodeEnumName(code), std::dynamic_pointer_cast<LabelCommand>(cmd));
    case EventCode::Jump_To_Label:
      return std::make_shared<Dialog_JumpToLabel>(DecodeEnumName(code), std::dynamic_pointer_cast<JumpToLabelCommand>(cmd));
    case EventCode::Control_Switches:
      return std::make_shared<Dialog_ControlSwitches>(DecodeEnumName(code), std::dynamic_pointer_cast<ControlSwitches>(cmd));
    case EventCode::Control_Variables:
      return std::make_shared<Dialog_ControlVariables>(DecodeEnumName(code), std::dynamic_pointer_cast<ControlVariables>(cmd), m_troopId);
    case EventCode::Control_Self_Switch:
      return std::make_shared<Dialog_ControlSelfSwitch>(DecodeEnumName(code), std::dynamic_pointer_cast<ControlSelfSwitchCommand>(cmd));
    case EventCode::Control_Timer:
      return std::make_shared<Dialog_ControlTimer>(DecodeEnumName(code), std::dynamic_pointer_cast<ControlTimerCommand>(cmd));
    case EventCode::Change_Gold:
      return std::make_shared<Dialog_ChangeGold>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeGoldCommand>(cmd));
    case EventCode::Change_Items:
      return std::make_shared<Dialog_ChangeItems>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeItemsCommand>(cmd));
    case EventCode::Change_Weapons:
      return std::make_shared<Dialog_ChangeWeapons>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeWeaponsCommand>(cmd));
    case EventCode::Change_Armors:
      return std::make_shared<Dialog_ChangeArmors>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeArmorsCommand>(cmd));
    case EventCode::Change_Party_Member:
      return std::make_shared<Dialog_ChangePartyMember>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangePartyMemberCommand>(cmd));
    case EventCode::Change_Battle_BGM:
      return std::make_shared<Dialog_ChangeBattleBGM>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeBattleBGMCommand>(cmd));
    case EventCode::Change_Victory_ME:
      return std::make_shared<Dialog_ChangeVictoryME>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeVictoryMECommand>(cmd));
    case EventCode::Change_Save_Access:
      return std::make_shared<Dialog_ChangeSaveAccess>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeSaveAccessCommand>(cmd));
    case EventCode::Change_Menu_Access:
      return std::make_shared<Dialog_ChangeMenuAccess>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeMenuAccessCommand>(cmd));
    case EventCode::Change_Encounter:
      return std::make_shared<Dialog_ChangeEncounter>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeEncounterDisableCommand>(cmd));
    case EventCode::Change_Formation_Access:
      return std::make_shared<Dialog_ChangeFormationAccess>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeFormationAccessCommand>(cmd));
    case EventCode::Change_Window_Color:
      return std::make_shared<Dialog_ChangeWindowColor>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeWindowColorCommand>(cmd));
    case EventCode::Change_Defeat_ME:
      return std::make_shared<Dialog_ChangeDefeatME>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeDefeatMECommand>(cmd));
    case EventCode::Change_Vehicle_BGM:
      return std::make_shared<Dialog_ChangeVehicleBGM>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeVehicleBGMCommand>(cmd));
    case EventCode::Transfer_Player:
      return std::make_shared<Dialog_TransferPlayer>(DecodeEnumName(code), std::dynamic_pointer_cast<TransferPlayerCommand>(cmd));
    case EventCode::Set_Vehicle_Location:
      return std::make_shared<Dialog_SetVehicleLocation>(DecodeEnumName(code), std::dynamic_pointer_cast<SetVehicleLocationCommand>(cmd));
    case EventCode::Set_Event_Location:
      return std::make_shared<Dialog_SetEventLocation>(DecodeEnumName(code), std::dynamic_pointer_cast<SetEventLocationCommand>(cmd));
    case EventCode::Scroll_Map:
      return std::make_shared<Dialog_ScrollMap>(DecodeEnumName(code), std::dynamic_pointer_cast<ScrollMapCommand>(cmd));
    case EventCode::Set_Movement_Route:
      return std::make_shared<Dialog_SetMovementRoute>(DecodeEnumName(code), std::dynamic_pointer_cast<SetMovementRouteCommand>(cmd));
    case EventCode::Movement_Route_Step:
      break;
    case EventCode::Get_on_fwsl_off_Vehicle:
      return std::make_shared<Dialog_GetOnOffVehicle>(DecodeEnumName(code), std::dynamic_pointer_cast<GetOnOffVehicleCommand>(cmd));
    case EventCode::Change_Transparency:
      return std::make_shared<Dialog_ChangeTransparency>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeTransparencyCommand>(cmd));
    case EventCode::Show_Animation:
      return std::make_shared<Dialog_ShowAnimation>(DecodeEnumName(code), std::dynamic_pointer_cast<ShowAnimationCommand>(cmd));
    case EventCode::Show_Balloon_Icon:
      return std::make_shared<Dialog_ShowBalloonIcon>(DecodeEnumName(code), std::dynamic_pointer_cast<ShowBalloonIconCommand>(cmd));
    case EventCode::Erase_Event:
      return std::make_shared<Dialog_EraseEvent>(DecodeEnumName(code), std::dynamic_pointer_cast<EraseEventCommand>(cmd));
    case EventCode::Change_Player_Followers:
      return std::make_shared<Dialog_ChangePlayerFollowers>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangePlayerFollowersCommand>(cmd));
    case EventCode::Gather_Followers:
      return std::make_shared<Dialog_GatherFollowers>(DecodeEnumName(code), std::dynamic_pointer_cast<GatherFollowersCommand>(cmd));
    case EventCode::Fadeout_Screen:
      return std::make_shared<Dialog_FadeoutScreen>(DecodeEnumName(code), std::dynamic_pointer_cast<FadeoutScreenCommand>(cmd));
    case EventCode::Fadein_Screen:
      return std::make_shared<Dialog_FadeinScreen>(DecodeEnumName(code), std::dynamic_pointer_cast<FadeinScreenCommand>(cmd));
    case EventCode::Tint_Screen:
      return std::make_shared<Dialog_TintScreen>(DecodeEnumName(code), std::dynamic_pointer_cast<TintScreenCommand>(cmd));
    case EventCode::Flash_Screen:
      return std::make_shared<Dialog_FlashScreen>(DecodeEnumName(code), std::dynamic_pointer_cast<FlashScreenCommand>(cmd));
    case EventCode::Shake_Screen:
      return std::make_shared<Dialog_ShakeScreen>(DecodeEnumName(code), std::dynamic_pointer_cast<ShakeScreenCommand>(cmd));
    case EventCode::Wait:
      return std::make_shared<Dialog_Wait>(DecodeEnumName(code), std::dynamic_pointer_cast<WaitCommand>(cmd));
    case EventCode::Show_Picture:
      return std::make_shared<Dialog_ShowPicture>(DecodeEnumName(code), std::dynamic_pointer_cast<ShowPictureCommand>(cmd));
    case EventCode::Move_Picture:
      return std::make_shared<Dialog_MovePicture>(DecodeEnumName(code), std::dynamic_pointer_cast<MovePictureCommand>(cmd));
    case EventCode::Rotate_Picture:
      return std::make_shared<Dialog_RotatePicture>(DecodeEnumName(code), std::dynamic_pointer_cast<RotatePictureCommand>(cmd));
    case EventCode::Tint_Picture:
      return std::make_shared<Dialog_TintPicture>(DecodeEnumName(code), std::dynamic_pointer_cast<TintPictureCommand>(cmd));
    case EventCode::Erase_Picture:
      return std::make_shared<Dialog_ErasePicture>(DecodeEnumName(code), std::dynamic_pointer_cast<ErasePictureCommand>(cmd));
    case EventCode::Set_Weather_Effect:
      return std::make_shared<Dialog_SetWeatherEffect>(DecodeEnumName(code), std::dynamic_pointer_cast<SetWeatherEffectCommand>(cmd));
    case EventCode::Play_BGM:
      return std::make_shared<Dialog_PlayBGM>(DecodeEnumName(code), std::dynamic_pointer_cast<PlayBGMCommand>(cmd));
    case EventCode::Fadeout_BGM:
      return std::make_shared<Dialog_FadeoutBGM>(DecodeEnumName(code), std::dynamic_pointer_cast<FadeoutBGM>(cmd));
    case EventCode::Save_BGM:
      return std::make_shared<Dialog_SaveBGM>(DecodeEnumName(code), std::dynamic_pointer_cast<SaveBGMCommand>(cmd));
    case EventCode::Resume_BGM:
      return std::make_shared<Dialog_ReplayBGM>(DecodeEnumName(code), std::dynamic_pointer_cast<ResumeBGMCommand>(cmd));
    case EventCode::Play_BGS:
      return std::make_shared<Dialog_PlayBGS>(DecodeEnumName(code), std::dynamic_pointer_cast<PlayBGSCommand>(cmd));
    case EventCode::Fadeout_BGS:
      return std::make_shared<Dialog_FadeoutBGS>(DecodeEnumName(code), std::dynamic_pointer_cast<FadeoutBGS>(cmd));
    case EventCode::Play_ME:
      return std::make_shared<Dialog_PlayME>(DecodeEnumName(code), std::dynamic_pointer_cast<PlayMECommand>(cmd));
    case EventCode::Play_SE:
      return std::make_shared<Dialog_PlaySE>(DecodeEnumName(code), std::dynamic_pointer_cast<PlaySECommand>(cmd));
    case EventCode::Stop_SE:
      return std::make_shared<Dialog_StopSE>(DecodeEnumName(code), std::dynamic_pointer_cast<StopSECommand>(cmd));
    case EventCode::Play_Movie:
      return std::make_shared<Dialog_PlayMovie>(DecodeEnumName(code), std::dynamic_pointer_cast<PlayMovieCommand>(cmd));
    case EventCode::Change_Map_Display_Name:
      return std::make_shared<Dialog_ChangeMapDisplayName>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeMapNameDisplayCommand>(cmd));
    case EventCode::Change_Tileset:
      return std::make_shared<Dialog_ChangeTileset>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeTilesetCommand>(cmd));
    case EventCode::Change_Battle_Back:
      return std::make_shared<Dialog_ChangeBattleBack>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeBattlebackCommand>(cmd));
    case EventCode::Change_Parallax:
      return std::make_shared<Dialog_ChangeParallax>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeParallaxCommand>(cmd));
    case EventCode::Get_Location_Info:
      // return std::make_shared<Dialog_GetLocationInfo>(DecodeEnumName(code),
      // std::dynamic_pointer_cast<GetLocationInfoCommand>(cmd));
      break;
    case EventCode::If_Win:
      break;
    case EventCode::If_Escape:
      break;
    case EventCode::If_Lose:
      break;
    case EventCode::End_del_Battle_Processing:
      break;
    case EventCode::Shop_Processing:
      return std::make_shared<Dialog_ShopProcessing>(DecodeEnumName(code), std::dynamic_pointer_cast<ShopProcessingCommand>(cmd));
    case EventCode::Shop_Processing_Good:
      break;
    case EventCode::Name_Input_Processing:
      return std::make_shared<Dialog_NameInputProcessing>(DecodeEnumName(code), std::dynamic_pointer_cast<NameInputCommand>(cmd));
    case EventCode::Change_HP:
      return std::make_shared<Dialog_ChangeHP>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeHPCommand>(cmd));
    case EventCode::Change_MP:
      return std::make_shared<Dialog_ChangeMP>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeMPCommand>(cmd));
    case EventCode::Change_State:
      return std::make_shared<Dialog_ChangeState>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeStateCommand>(cmd));
    case EventCode::Recover_All:
      return std::make_shared<Dialog_RecoverAll>(DecodeEnumName(code), std::dynamic_pointer_cast<RecoverAllCommand>(cmd));
    case EventCode::Change_EXP:
      return std::make_shared<Dialog_ChangeEXP>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeEXPCommand>(cmd));
    case EventCode::Change_Level:
      return std::make_shared<Dialog_ChangeLevel>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeLevelCommand>(cmd));
    case EventCode::Change_Parameter:
      return std::make_shared<Dialog_ChangeParameter>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeParameterCommand>(cmd));
    case EventCode::Change_Skill:
      return std::make_shared<Dialog_ChangeSkill>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeSkillCommand>(cmd));
    case EventCode::Change_Equipment:
      return std::make_shared<Dialog_ChangeEquipment>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeEquipmentCommand>(cmd));
    case EventCode::Change_Name:
      return std::make_shared<Dialog_ChangeName>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeNameCommand>(cmd));
    case EventCode::Change_Class:
      return std::make_shared<Dialog_ChangeClass>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeClassCommand>(cmd));
    case EventCode::Change_Actor_Images:
      return std::make_shared<Dialog_ChangeActorImages>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeActorImageCommand>(cmd));
    case EventCode::Change_Vehicle_Image:
      return std::make_shared<Dialog_ChangeVehicleImage>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeVehicleImageCommand>(cmd));
    case EventCode::Change_Nickname:
      return std::make_shared<Dialog_ChangeNickname>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeNicknameCommand>(cmd));
    case EventCode::Change_Profile:
      return std::make_shared<Dialog_ChangeProfile>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeProfileCommand>(cmd));
    case EventCode::Change_TP:
      return std::make_shared<Dialog_ChangeTP>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeTPCommand>(cmd));
    case EventCode::Enemy_Recover_All:
      return std::make_shared<Dialog_EnemyRecoverAll>(DecodeEnumName(code), std::dynamic_pointer_cast<EnemyRecoverAllCommand>(cmd), m_troopId);
    case EventCode::Enemy_Appear:
      return std::make_shared<Dialog_EnemyAppear>(DecodeEnumName(code), std::dynamic_pointer_cast<EnemyAppearCommand>(cmd), m_troopId);
    case EventCode::Enemy_Transform:
      return std::make_shared<Dialog_EnemyTransform>(DecodeEnumName(code), std::dynamic_pointer_cast<EnemyTransformCommand>(cmd), m_troopId);
    case EventCode::Change_Enemy_HP:
      return std::make_shared<Dialog_ChangeEnemyHP>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeEnemyHPCommand>(cmd), m_troopId);
    case EventCode::Change_Enemy_MP:
      return std::make_shared<Dialog_ChangeEnemyMP>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeEnemyMPCommand>(cmd), m_troopId);
    case EventCode::Change_Enemy_TP:
      return std::make_shared<Dialog_ChangeEnemyTP>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeEnemyTPCommand>(cmd), m_troopId);
    case EventCode::Change_Enemy_State:
      return std::make_shared<Dialog_ChangeEnemyState>(DecodeEnumName(code), std::dynamic_pointer_cast<ChangeEnemyStateCommand>(cmd), m_troopId);
    case EventCode::Show_Battle_Animation:
      return std::make_shared<Dialog_ShowBattleAnimation>(DecodeEnumName(code), std::dynamic_pointer_cast<ShowBattleAnimationCommand>(cmd), m_troopId);
    case EventCode::Force_Action:
      // return std::make_shared<Dialog_ForceAction>(DecodeEnumName(code),
      // std::dynamic_pointer_cast<ForceActionCommand>(cmd));
      break;
    case EventCode::Abort_Battle:
      return std::make_shared<Dialog_AbortBattle>(DecodeEnumName(code), std::dynamic_pointer_cast<AbortBattleCommand>(cmd));
    case EventCode::Open_Menu_Screen:
      return std::make_shared<Dialog_OpenMenuScreen>(DecodeEnumName(code), std::dynamic_pointer_cast<OpenMenuCommand>(cmd));
    case EventCode::Open_Save_Screen:
      return std::make_shared<Dialog_OpenSaveScreen>(DecodeEnumName(code), std::dynamic_pointer_cast<OpenSaveCommand>(cmd));
    case EventCode::Game_Over:
      return std::make_shared<Dialog_GameOver>(DecodeEnumName(code), std::dynamic_pointer_cast<GameOverCommand>(cmd));
    case EventCode::Return_To_Title_Screen:
      return std::make_shared<Dialog_ReturnToTitle>(DecodeEnumName(code), std::dynamic_pointer_cast<ReturnToTitleCommand>(cmd));
    case EventCode::Script:
      return std::make_shared<Dialog_Script>(DecodeEnumName(code), std::dynamic_pointer_cast<ScriptCommand>(cmd));
    case EventCode::Next_Script:
      break;
    case EventCode::Plugin_Command_del_MV:
      return std::make_shared<Dialog_ChangePluginCommand>(DecodeEnumName(code), std::dynamic_pointer_cast<PluginCommandMV>(cmd));
    case EventCode::Plugin_Command_del_MZ:
      break;
    case EventCode::Next_PluginMZ_Command:
      break;
    case EventCode::Move_Down:
      break;
    case EventCode::Move_Left:
      break;
    case EventCode::Move_Right:
      break;
    case EventCode::Move_Up:
      break;
    case EventCode::Move_Lower_Left:
      break;
    case EventCode::Move_Lower_Right:
      break;
    case EventCode::Move_Upper_Left:
      break;
    case EventCode::Move_Upper_Right:
      break;
    case EventCode::Move_at_Random:
      break;
    case EventCode::Move_toward_Player:
      break;
    case EventCode::Move_away_from_Player:
      break;
    case EventCode::_1_Step_Forward:
      break;
    case EventCode::_1_Step_Backward:
      break;
    case EventCode::Jump:
      break;
    case EventCode::Wait_del_Movement:
      break;
    case EventCode::Turn_Down:
      break;
    case EventCode::Turn_Left:
      break;
    case EventCode::Turn_Right:
      break;
    case EventCode::Turn_Up:
      break;
    case EventCode::Turn_90_deg_Right:
      break;
    case EventCode::Turn_90_deg_Left:
      break;
    case EventCode::Turn_180_deg:
      break;
    case EventCode::Turn_90_deg_Left_or_Right:
      break;
    case EventCode::Turn_at_Random:
      break;
    case EventCode::Turn_toward_Player:
      break;
    case EventCode::Turn_away_from_Player:
      break;
    case EventCode::Switch_ON:
      break;
    case EventCode::Switch_OFF:
      break;
    case EventCode::Speed:
      break;
    case EventCode::Frequency:
      break;
    case EventCode::Walking_Animation_ON:
      break;
    case EventCode::Walking_Animation_OFF:
      break;
    case EventCode::Stepping_Animation_ON:
      break;
    case EventCode::Stepping_Animation_OFF:
      break;
    case EventCode::Direction_Fix_ON:
      break;
    case EventCode::Direction_Fix_OFF:
      break;
    case EventCode::Through_ON:
      break;
    case EventCode::Through_OFF:
      break;
    case EventCode::Transparent_ON:
      break;
    case EventCode::Transparent_OFF:
      break;
    case EventCode::Change_Image:
      break;
    case EventCode::Change_Opacity:
      break;
    case EventCode::Change_Blend_Mode:
      break;
    case EventCode::Play_SE_del_Movement:
      break;
    case EventCode::Script_del_Movement:
      break;
    }
    return {};
  }
};