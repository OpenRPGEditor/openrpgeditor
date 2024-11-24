#include "Database/CommandParser.hpp"

#include "Database/EventCommands.hpp"

#include <iostream>

using json = nlohmann::ordered_json;

std::vector<std::shared_ptr<IEventCommand>> CommandParser::parse(const json& _json) {
  std::vector<std::shared_ptr<IEventCommand>> ret;
  parser = _json;

  while (index < parser.size()) {
    EventCode code = EventCode::Event_Dummy;
    currentCommand().at("code").get_to(code);
    const auto parameters = currentCommand().value("parameters", nlohmann::json());
    const auto indent = currentCommand().value("indent", std::optional<int>{});

    // std::cout << "Processing: " << magic_enum::enum_name(code) << " (" << static_cast<int>(code) << ")" << std::endl;
    switch (code) {
    case EventCode::Event_Dummy:
      ret.emplace_back(new EventDummy(indent, parameters));
      break;
    case EventCode::Show_Text: {
      auto text = dynamic_cast<ShowTextCommand*>(ret.emplace_back(new ShowTextCommand(indent, parameters)).get());
      while (nextEventCommand() == EventCode::Next_Text) {
        ++index;
        auto nextIndent = currentCommand().value("indent", std::optional<int>{});
        auto nextParameters = currentCommand().value("parameters", nlohmann::json{});
        text->addText(new NextTextCommand(nextIndent, nextParameters));
      }
      break;
    }
    case EventCode::Show_Choices:
      ret.emplace_back(new ShowChoiceCommand(indent, parameters));
      break;

    case EventCode::When_Selected:
      ret.emplace_back(new WhenSelectedCommand(indent, parameters));
      break;
    case EventCode::When_Cancel:
      ret.emplace_back(new WhenCancelCommand(indent, parameters));
      break;
    case EventCode::End_del_ShowChoices:
      ret.emplace_back(new ShowChoicesEndCommand(indent, parameters));
      break;
    case EventCode::Input_Number:
      ret.emplace_back(new InputNumberCommand(indent, parameters));
      break;
    case EventCode::Select_Item:
      ret.emplace_back(new SelectItemCommand(indent, parameters));
      break;
    case EventCode::Show_Scrolling_Text: {
      auto text = dynamic_cast<ShowScrollTextCommand*>(ret.emplace_back(new ShowScrollTextCommand(indent, parameters)).get());
      while (nextEventCommand() == EventCode::Next_Scrolling_Text) {
        ++index;
        auto nextIndent = currentCommand().value("indent", std::optional<int>{});
        auto nextParameters = currentCommand().value("parameters", nlohmann::json{});
        text->addText(new NextScrollingTextCommand(nextIndent, nextParameters));
      }
      break;
    }
    case EventCode::Comment: {
      auto text = dynamic_cast<CommentCommand*>(ret.emplace_back(new CommentCommand(indent, parameters)).get());
      while (nextEventCommand() == EventCode::Next_Comment) {
        ++index;
        auto nextIndent = currentCommand().value("indent", std::optional<int>{});
        auto nextParameters = currentCommand().value("parameters", nlohmann::json{});
        text->addText(new NextCommentCommand(nextIndent, nextParameters));
      }
      break;
    }
    case EventCode::Conditional_Branch:
      ret.emplace_back(new ConditionalBranchCommand(indent, parameters));
      break;
    case EventCode::Else:
      ret.emplace_back(new ElseCommand(indent, parameters));
      break;
    case EventCode::Loop:
      ret.emplace_back(new LoopCommand(indent, parameters));
      break;
    case EventCode::Repeat_Above:
      ret.emplace_back(new RepeatAboveCommand(indent, parameters));
      break;
    case EventCode::Break_Loop:
      ret.emplace_back(new BreakLoopCommand(indent, parameters));
      break;
    case EventCode::Exit_Event_Processing:
      ret.emplace_back(new ExitEventProcessingCommand(indent, parameters));
      break;
    case EventCode::Common_Event:
      ret.emplace_back(new CommonEventCommand(indent, parameters));
      break;
    case EventCode::Label:
      ret.emplace_back(new LabelCommand(indent, parameters));
      break;
    case EventCode::Jump_To_Label:
      ret.emplace_back(new JumpToLabelCommand(indent, parameters));
      break;
    case EventCode::Control_Switches:
      ret.emplace_back(new ControlSwitches(indent, parameters));
      break;
    case EventCode::Control_Variables:
      ret.emplace_back(new ControlVariables(indent, parameters));
      break;
    case EventCode::Control_Self_Switch:
      ret.emplace_back(new ControlSelfSwitchCommand(indent, parameters));
      break;
    case EventCode::Control_Timer:
      ret.emplace_back(new ControlTimerCommand(indent, parameters));
      break;
    case EventCode::Change_Gold:
      ret.emplace_back(new ChangeGoldCommand(indent, parameters));
      break;
    case EventCode::Change_Items:
      ret.emplace_back(new ChangeItemsCommand(indent, parameters));
      break;
    case EventCode::Change_Weapons:
      ret.emplace_back(new ChangeWeaponsCommand(indent, parameters));
      break;
    case EventCode::Change_Armors:
      ret.emplace_back(new ChangeArmorsCommand(indent, parameters));
      break;
    case EventCode::Change_Party_Member:
      ret.emplace_back(new ChangePartyMemberCommand(indent, parameters));
      break;
    case EventCode::Change_Battle_BGM:
      ret.emplace_back(new ChangeBattleBGMCommand(indent, parameters));
      break;
    case EventCode::Change_Victory_ME:
      ret.emplace_back(new ChangeVictoryMECommand(indent, parameters));
      break;
    case EventCode::Change_Save_Access:
      ret.emplace_back(new ChangeSaveAccessCommand(indent, parameters));
      break;
    case EventCode::Change_Menu_Access:
      ret.emplace_back(new ChangeMenuAccessCommand(indent, parameters));
      break;
    case EventCode::Change_Encounter_Disable:
      ret.emplace_back(new ChangeEncounterDisableCommand(indent, parameters));
      break;
    case EventCode::Change_Formation_Access:
      ret.emplace_back(new ChangeFormationAccessCommand(indent, parameters));
      break;
    case EventCode::Change_Window_Color:
      ret.emplace_back(new ChangeWindowColorCommand(indent, parameters));
      break;
    case EventCode::Change_Defeat_ME:
      ret.emplace_back(new ChangeDefeatMECommand(indent, parameters));
      break;
    case EventCode::Change_Vechicle_BGM:
      ret.emplace_back(new ChangeVehicleBGMCommand(indent, parameters));
      break;
    case EventCode::Transfer_Player:
      ret.emplace_back(new TransferPlayerCommand(indent, parameters));
      break;
    case EventCode::Set_Vehicle_Location:
      ret.emplace_back(new SetVehicleLocationCommand(indent, parameters));
      break;
    case EventCode::Set_Event_Location:
      ret.emplace_back(new SetEventLocationCommand(indent, parameters));
      break;
    case EventCode::Scroll_Map:
      ret.emplace_back(new ScrollMapCommand(indent, parameters));
      break;
    case EventCode::Set_Movement_Route: {
      auto route = dynamic_cast<SetMovementRouteCommand*>(ret.emplace_back(new SetMovementRouteCommand(indent, parameters)).get());
      while (nextEventCommand() == EventCode::Movement_Route_Step) {
        ++index;
        auto nextIndent = currentCommand().value("indent", std::optional<int>{});
        auto nextParameters = currentCommand().value("parameters", nlohmann::json{});
        route->addStep(new MovementRouteStepCommand(nextIndent, nextParameters));
      }
      break;
    }
    case EventCode::Get_On_Off_Vehicle:
      ret.emplace_back(new GetOnOffVehicleCommand(indent, parameters));
      break;
    case EventCode::Change_Transparency:
      ret.emplace_back(new ChangeTransparencyCommand(indent, parameters));
      break;
    case EventCode::Show_Animation:
      ret.emplace_back(new ShowAnimationCommand(indent, parameters));
      break;
    case EventCode::Show_Balloon_Icon:
      ret.emplace_back(new ShowBalloonIconCommand(indent, parameters));
      break;
    case EventCode::Erase_Event:
      ret.emplace_back(new EraseEventCommand(indent, parameters));
      break;
    case EventCode::Change_Player_Followers:
      ret.emplace_back(new ChangePlayerFollowersCommand(indent, parameters));
      break;
    case EventCode::Gather_Followers:
      ret.emplace_back(new GatherFollowersCommand(indent, parameters));
      break;
    case EventCode::Fadeout_Screen:
      ret.emplace_back(new FadeoutScreenCommand(indent, parameters));
      break;
    case EventCode::Fadein_Screen:
      ret.emplace_back(new FadeinScreenCommand(indent, parameters));
      break;
    case EventCode::Tint_Screen:
      ret.emplace_back(new TintScreenCommand(indent, parameters));
      break;
    case EventCode::Flash_Screen:
      ret.emplace_back(new FlashScreenCommand(indent, parameters));
      break;
    case EventCode::Shake_Screen:
      ret.emplace_back(new ShakeScreenCommand(indent, parameters));
      break;
    case EventCode::Wait:
      ret.emplace_back(new WaitCommand(indent, parameters));
      break;
    case EventCode::Show_Picture:
      ret.emplace_back(new ShowPictureCommand(indent, parameters));
      break;
    case EventCode::Move_Picture:
      ret.emplace_back(new MovePictureCommand(indent, parameters));
      break;
    case EventCode::Rotate_Picture:
      ret.emplace_back(new RotatePictureCommand(indent, parameters));
      break;
    case EventCode::Tint_Picture:
      ret.emplace_back(new TintPictureCommand(indent, parameters));
      break;
    case EventCode::Set_Weather_Effect:
      ret.emplace_back(new SetWeatherEffectCommand(indent, parameters));
      break;
    case EventCode::Erase_Picture:
      ret.emplace_back(new ErasePictureCommand(indent, parameters));
      break;
    case EventCode::Play_BGM:
      ret.emplace_back(new PlayBGMCommand(indent, parameters));
      break;
    case EventCode::Fadeout_BGM:
      ret.emplace_back(new FadeoutBGM(indent, parameters));
      break;
    case EventCode::Save_BGM:
      ret.emplace_back(new SaveBGMCommand(indent, parameters));
      break;
    case EventCode::Resume_BGM:
      ret.emplace_back(new ResumeBGMCommand(indent, parameters));
      break;
    case EventCode::Play_BGS:
      ret.emplace_back(new PlayBGSCommand(indent, parameters));
      break;
    case EventCode::Fadeout_BGS:
      ret.emplace_back(new FadeoutBGS(indent, parameters));
      break;
    case EventCode::Play_ME:
      ret.emplace_back(new PlayMECommand(indent, parameters));
      break;
    case EventCode::Play_SE:
      ret.emplace_back(new PlaySECommand(indent, parameters));
      break;
    case EventCode::Stop_SE:
      ret.emplace_back(new StopSECommand(indent, parameters));
      break;
    case EventCode::Play_Movie:
      ret.emplace_back(new PlayMovieCommand(indent, parameters));
      break;
    case EventCode::Change_Map_Name_Display:
      ret.emplace_back(new ChangeMapNameDisplayCommand(indent, parameters));
      break;
    case EventCode::Change_Tileset:
      ret.emplace_back(new ChangeTilesetCommand(indent, parameters));
      break;
    case EventCode::Change_Battle_Back:
      ret.emplace_back(new ChangeBattlebackCommand(indent, parameters));
      break;
    case EventCode::Change_Parallax:
      ret.emplace_back(new ChangeParallaxCommand(indent, parameters));
      break;
    case EventCode::Get_Location_Info:
      ret.emplace_back(new GetLocationInfoCommand(indent, parameters));
      break;
    case EventCode::Battle_Processing:
      ret.emplace_back(new BattleProcessingCommand(indent, parameters));
      break;
    case EventCode::If_Win:
      ret.emplace_back(new IfWinCommand(indent, parameters));
      break;
    case EventCode::If_Escape:
      ret.emplace_back(new IfEscapeCommand(indent, parameters));
      break;
    case EventCode::If_Lose:
      ret.emplace_back(new IfLoseCommand(indent, parameters));
      break;
    case EventCode::End_del_Battle_Processing:
      ret.emplace_back(new EndBattleProcessingCommand(indent, parameters));
      break;
    case EventCode::Shop_Processing: {
      auto* shop = dynamic_cast<ShopProcessingCommand*>(ret.emplace_back(new ShopProcessingCommand(indent, parameters)).get());
      while (nextEventCommand() == EventCode::Shop_Processing_Good) {
        ++index;
        auto nextIndent = currentCommand().value("indent", std::optional<int>{});
        auto nextParameters = currentCommand().value("parameters", nlohmann::json{});
        shop->addGood(new ShopProcessingGoodCommand(nextIndent, nextParameters));
      }
      break;
    }
    case EventCode::Name_Input_Processing:
      ret.emplace_back(new NameInputCommand(indent, parameters));
      break;
    case EventCode::Change_HP:
      ret.emplace_back(new ChangeHPCommand(indent, parameters));
      break;
    case EventCode::Change_MP:
      ret.emplace_back(new ChangeMPCommand(indent, parameters));
      break;
    case EventCode::Change_TP:
      ret.emplace_back(new ChangeTPCommand(indent, parameters));
      break;
    case EventCode::Change_EXP:
      ret.emplace_back(new ChangeEXPCommand(indent, parameters));
      break;
    case EventCode::Change_Level:
      ret.emplace_back(new ChangeLevelCommand(indent, parameters));
      break;
    case EventCode::Change_Parameter:
      ret.emplace_back(new ChangeParameterCommand(indent, parameters));
      break;
    case EventCode::Recover_All:
      ret.emplace_back(new RecoverAllCommand(indent, parameters));
      break;
    case EventCode::Change_Name:
      ret.emplace_back(new ChangeNameCommand(indent, parameters));
      break;
    case EventCode::Change_Class:
      ret.emplace_back(new ChangeClassCommand(indent, parameters));
      break;
    case EventCode::Change_State:
      ret.emplace_back(new ChangeStateCommand(indent, parameters));
      break;
    case EventCode::Change_Skill:
      ret.emplace_back(new ChangeSkillCommand(indent, parameters));
      break;
    case EventCode::Change_Equipment:
      ret.emplace_back(new ChangeEquipmentCommand(indent, parameters));
      break;
    case EventCode::Change_Profile:
      ret.emplace_back(new ChangeProfileCommand(indent, parameters));
      break;
    case EventCode::Change_Nickname:
      ret.emplace_back(new ChangeNicknameCommand(indent, parameters));
      break;
    case EventCode::Game_Over:
      ret.emplace_back(new GameOverCommand(indent, parameters));
      break;
    case EventCode::Return_To_Title_Screen:
      ret.emplace_back(new ReturnToTitleCommand(indent, parameters));
      break;
    case EventCode::Open_Menu_Screen:
      ret.emplace_back(new OpenMenuCommand(indent, parameters));
      break;
    case EventCode::Open_Save_Screen:
      ret.emplace_back(new OpenSaveCommand(indent, parameters));
      break;
    case EventCode::Abort_Battle:
      ret.emplace_back(new AbortBattleCommand(indent, parameters));
      break;
    case EventCode::Enemy_Recover_All:
      ret.emplace_back(new EnemyRecoverAllCommand(indent, parameters));
      break;
    case EventCode::Enemy_Appear:
      ret.emplace_back(new EnemyAppearCommand(indent, parameters));
      break;
    case EventCode::Enemy_Transform:
      ret.emplace_back(new EnemyTransformCommand(indent, parameters));
      break;
    case EventCode::Change_Enemy_HP:
      ret.emplace_back(new ChangeEnemyHPCommand(indent, parameters));
      break;
    case EventCode::Change_Enemy_MP:
      ret.emplace_back(new ChangeEnemyMPCommand(indent, parameters));
      break;
    case EventCode::Change_Enemy_TP:
      ret.emplace_back(new ChangeEnemyTPCommand(indent, parameters));
      break;
    case EventCode::Change_Enemy_State:
      ret.emplace_back(new ChangeEnemyStateCommand(indent, parameters));
      break;
    case EventCode::Force_Action:
      ret.emplace_back(new ForceActionCommand(indent, parameters));
      break;
    case EventCode::Show_Battle_Animation:
      ret.emplace_back(new ShowBattleAnimationCommand(indent, parameters));
      break;
    case EventCode::Change_Vehicle_Image:
      ret.emplace_back(new ChangeVehicleImageCommand(indent, parameters));
      break;
    case EventCode::Change_Actor_Images:
      ret.emplace_back(new ChangeActorImageCommand(indent, parameters));
      break;
    case EventCode::Script: {
      auto end = dynamic_cast<ScriptCommand*>(ret.emplace_back(new ScriptCommand(indent, parameters)).get());
      while (nextEventCommand() == EventCode::Next_Script) {
        ++index;
        auto nextIndent = currentCommand().value("indent", std::optional<int>{});
        auto nextParameters = currentCommand().value("parameters", nlohmann::json{});
        end->addText(new NextScriptCommand(nextIndent, nextParameters));
      }
      break;
    }
    case EventCode::PluginMV_Command:
      ret.emplace_back(new PluginCommandMV(indent, parameters));
      break;
    case EventCode::End:
      ret.emplace_back(new EndCommand(indent, parameters));
      break;
      // MovementRoute commands
    case EventCode::Move_Down:
      ret.emplace_back(new MovementMoveDownCommand(indent, parameters));
      break;
    case EventCode::Move_Left:
      ret.emplace_back(new MovementMoveLeftCommand(indent, parameters));
      break;
    case EventCode::Move_Right:
      ret.emplace_back(new MovementMoveRightCommand(indent, parameters));
      break;
    case EventCode::Move_Up:
      ret.emplace_back(new MovementMoveUpCommand(indent, parameters));
      break;
    case EventCode::Move_Lower_Left:
      ret.emplace_back(new MovementMoveLowerLeftCommand(indent, parameters));
      break;
    case EventCode::Move_Lower_Right:
      ret.emplace_back(new MovementMoveLowerRightCommand(indent, parameters));
      break;
    case EventCode::Move_Upper_Left:
      ret.emplace_back(new MovementMoveUpperLeftCommand(indent, parameters));
      break;
    case EventCode::Move_Upper_Right:
      ret.emplace_back(new MovementMoveUpperRightCommand(indent, parameters));
      break;
    case EventCode::Move_at_Random:
      ret.emplace_back(new MovementMoveAtRandomCommand(indent, parameters));
      break;
    case EventCode::Move_toward_Player:
      ret.emplace_back(new MovementMoveTowardPlayerCommand(indent, parameters));
      break;
    case EventCode::Move_away_from_Player:
      ret.emplace_back(new MovementMoveAwayFromPlayerCommand(indent, parameters));
      break;
    case EventCode::_1_Step_Forward:
      ret.emplace_back(new MovementMove1StepForwardCommand(indent, parameters));
      break;
    case EventCode::_1_Step_Backward:
      ret.emplace_back(new MovementMove1StepBackwardCommand(indent, parameters));
      break;
    case EventCode::Jump:
      ret.emplace_back(new MovementJumpCommand(indent, parameters));
      break;
    case EventCode::Wait_del_Movement:
      ret.emplace_back(new MovementWaitCommand(indent, parameters));
      break;
    case EventCode::Turn_Down:
      ret.emplace_back(new MovementTurnDownCommand(indent, parameters));
      break;
    case EventCode::Turn_Left:
      ret.emplace_back(new MovementTurnLeftCommand(indent, parameters));
      break;
    case EventCode::Turn_Right:
      ret.emplace_back(new MovementTurnRightCommand(indent, parameters));
      break;
    case EventCode::Turn_Up:
      ret.emplace_back(new MovementTurnUpCommand(indent, parameters));
      break;
    case EventCode::Turn_90_deg_Right:
      ret.emplace_back(new MovementTurn90DegRightCommand(indent, parameters));
      break;
    case EventCode::Turn_90_deg_Left:
      ret.emplace_back(new MovementTurn90DegLeftCommand(indent, parameters));
      break;
    case EventCode::Turn_180_deg:
      ret.emplace_back(new MovementTurn180DegCommand(indent, parameters));
      break;
    case EventCode::Turn_90_deg_Left_or_Right:
      ret.emplace_back(new MovementTurn90DegLeftOrRightCommand(indent, parameters));
      break;
    case EventCode::Turn_at_Random:
      ret.emplace_back(new MovementTurnAtRandomCommand(indent, parameters));
      break;
    case EventCode::Turn_toward_Player:
      ret.emplace_back(new MovementTurnTowardPlayerCommand(indent, parameters));
      break;
    case EventCode::Turn_away_from_Player:
      ret.emplace_back(new MovementTurnAwayFromPlayerCommand(indent, parameters));
      break;
    case EventCode::Switch_ON:
      ret.emplace_back(new MovementSwitchONCommand(indent, parameters));
      break;
    case EventCode::Switch_OFF:
      ret.emplace_back(new MovementSwitchOFFCommand(indent, parameters));
      break;
    case EventCode::Speed:
      ret.emplace_back(new MovementSpeedCommand(indent, parameters));
      break;
    case EventCode::Frequency:
      ret.emplace_back(new MovementFrequencyCommand(indent, parameters));
      break;
    case EventCode::Walking_Animation_ON:
      ret.emplace_back(new MovementWalkingAnimationONCommand(indent, parameters));
      break;
    case EventCode::Walking_Animation_OFF:
      ret.emplace_back(new MovementWalkingAnimationOFFCommand(indent, parameters));
      break;
    case EventCode::Stepping_Animation_ON:
      ret.emplace_back(new MovementSteppingAnimationONCommand(indent, parameters));
      break;
    case EventCode::Stepping_Animation_OFF:
      ret.emplace_back(new MovementSteppingAnimationOFFCommand(indent, parameters));
      break;
    case EventCode::Direction_Fix_ON:
      ret.emplace_back(new MovementDirectionFixONCommand(indent, parameters));
      break;
    case EventCode::Direction_Fix_OFF:
      ret.emplace_back(new MovementDirectionFixOFFCommand(indent, parameters));
      break;
    case EventCode::Through_ON:
      ret.emplace_back(new MovementThroughONCommand(indent, parameters));
      break;
    case EventCode::Through_OFF:
      ret.emplace_back(new MovementThroughOFFCommand(indent, parameters));
      break;
    case EventCode::Transparent_ON:
      ret.emplace_back(new MovementTransparentONCommand(indent, parameters));
      break;
    case EventCode::Transparent_OFF:
      ret.emplace_back(new MovementTransparentOFFCommand(indent, parameters));
      break;
    case EventCode::Change_Image:
      ret.emplace_back(new MovementChangeImageCommand(indent, parameters));
      break;
    case EventCode::Change_Opacity:
      ret.emplace_back(new MovementChangeOpacityCommand(indent, parameters));
      break;
    case EventCode::Change_Blend_Mode:
      ret.emplace_back(new MovementChangeBlendModeCommand(indent, parameters));
      break;
    case EventCode::Play_SE_del_Movement:
      ret.emplace_back(new MovementPlaySECommand(indent, parameters));
      break;
    case EventCode::Script_del_Movement: {
      ret.emplace_back(new MovementScriptCommand(indent, parameters));
      break;
    }
    default:
      UnhandledEventCommand* end = dynamic_cast<UnhandledEventCommand*>(ret.emplace_back(new UnhandledEventCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      end->m_code = code;
      end->data = parser[index];
      std::cout << "Unhandled command: " << magic_enum::enum_name(code) << " (" << static_cast<int>(code) << ")" << std::endl;
      break;
    }
    ++index;
  }
  return ret;
}

void CommandParser::serialize(nlohmann::ordered_json& data, const std::vector<std::shared_ptr<IEventCommand>>& list, bool movementRoute) {
  for (const auto& cmd : list) {
    if (!cmd) {
      continue;
    }
    const auto code = cmd->code();
    bool doParameters = true;
    if ((code >= EventCode::Move_Down && code <= EventCode::Script_del_Movement) || movementRoute) {
      doParameters = code == EventCode::Jump || code == EventCode::Change_Blend_Mode || code == EventCode::Change_Image || code == EventCode::Change_Opacity || code == EventCode::Frequency ||
                     code == EventCode::Speed || code == EventCode::Play_SE_del_Movement || code == EventCode::Wait_del_Movement || code == EventCode::Script_del_Movement ||
                     code == EventCode::Switch_ON || code == EventCode::Switch_OFF || code == EventCode::Event_Dummy;
    }
    cmd->serialize(data.emplace_back(), movementRoute ? code != EventCode::Event_Dummy : true, doParameters);

    switch (code) {
    case EventCode::Show_Scrolling_Text: {
      auto text = dynamic_cast<ShowScrollTextCommand*>(cmd.get());
      for (const auto& next : text->text) {
        if (!next) {
          continue;
        }
        next->serialize(data.emplace_back());
      }
      break;
    }
    case EventCode::Show_Text: {
      auto text = dynamic_cast<ShowTextCommand*>(cmd.get());
      for (const auto& next : text->text) {
        if (!next) {
          continue;
        }
        next->serialize(data.emplace_back());
      }
      break;
    }
    case EventCode::Comment: {
      auto text = dynamic_cast<CommentCommand*>(cmd.get());
      for (const auto& next : text->nextComments) {
        if (!next) {
          continue;
        }
        next->serialize(data.emplace_back());
      }
      break;
    }
    case EventCode::Script: {
      auto text = dynamic_cast<ScriptCommand*>(cmd.get());
      for (const auto& next : text->moreScript) {
        if (!next) {
          continue;
        }
        next->serialize(data.emplace_back());
      }
      break;
    }
    case EventCode::Set_Movement_Route: {
      auto route = dynamic_cast<SetMovementRouteCommand*>(cmd.get());
      for (const auto& next : route->editNodes) {
        if (!next) {
          continue;
        }
        next->serialize(data.emplace_back());
      }
      break;
    }
    default:
      break;
    }
  }
}
