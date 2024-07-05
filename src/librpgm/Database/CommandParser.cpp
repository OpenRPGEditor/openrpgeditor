#include "Database/CommandParser.hpp"

#include "Database/EventCommands.hpp"

#include <iostream>

using json = nlohmann::json;

std::vector<std::shared_ptr<IEventCommand>> CommandParser::parse(const json& _json) {
  std::vector<std::shared_ptr<IEventCommand>> ret;
  parser = _json;

  while (index < parser.size()) {
    EventCode code = EventCode::Event_Dummy;

    currentCommand().at("code").get_to(code);
    auto indent = currentCommand().value("indent", std::optional<int>{});
    auto parameters = currentCommand().value("parameters", nlohmann::json{});
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
      auto text =
          dynamic_cast<ShowScrollTextCommand*>(ret.emplace_back(new ShowScrollTextCommand(indent, parameters)).get());
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
      auto route = dynamic_cast<SetMovementRouteCommand*>(
          ret.emplace_back(new SetMovementRouteCommand(indent, parameters)).get());
      route->indent = parser[index].value("indent", std::optional<int>{});
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
    case EventCode::Show_Animation: {
      ret.emplace_back(new ShowAnimationCommand(indent, parameters));
      break;
    }
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
    case EventCode::Play_ME: {
      PlayMECommand* me = dynamic_cast<PlayMECommand*>(ret.emplace_back(new PlayMECommand()).get());
      me->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(me->audio);
      break;
    }
    case EventCode::Play_SE: {
      PlaySECommand* se = dynamic_cast<PlaySECommand*>(ret.emplace_back(new PlaySECommand()).get());
      se->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(se->audio);
      break;
    }
    case EventCode::Stop_SE: {
      StopSECommand* se = dynamic_cast<StopSECommand*>(ret.emplace_back(new StopSECommand()).get());
      se->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Play_Movie: {
      PlayMovieCommand* mov = dynamic_cast<PlayMovieCommand*>(ret.emplace_back(new PlayMovieCommand()).get());
      mov->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(mov->name);
      break;
    }
    case EventCode::Change_Map_Name_Display: {
      ChangeMapNameDisplayCommand* disp =
          dynamic_cast<ChangeMapNameDisplayCommand*>(ret.emplace_back(new ChangeMapNameDisplayCommand()).get());
      disp->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(disp->checkIfOn);
      break;
    }
    case EventCode::Change_Tileset: {
      ChangeTilesetCommand* ts =
          dynamic_cast<ChangeTilesetCommand*>(ret.emplace_back(new ChangeTilesetCommand()).get());
      ts->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(ts->tileset);
      break;
    }
    case EventCode::Change_Battle_Back: {
      ChangeBattlebackCommand* bbk =
          dynamic_cast<ChangeBattlebackCommand*>(ret.emplace_back(new ChangeBattlebackCommand()).get());
      bbk->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(bbk->battleBack1Name);
      parameters[1].get_to(bbk->battleBack2Name);
      break;
    }
    case EventCode::Change_Parallax: {
      ChangeParallaxCommand* parallax =
          dynamic_cast<ChangeParallaxCommand*>(ret.emplace_back(new ChangeParallaxCommand()).get());
      parallax->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(parallax->image);
      parameters[1].get_to(parallax->loopHorizontally);
      parameters[2].get_to(parallax->loopVertically);
      parameters[3].get_to(parallax->scrollX);
      parameters[4].get_to(parallax->scrollY);
      break;
    }
    case EventCode::Get_Location_Info: {
      GetLocationInfoCommand* loc =
          dynamic_cast<GetLocationInfoCommand*>(ret.emplace_back(new GetLocationInfoCommand()).get());
      loc->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(loc->variable);
      parameters[1].get_to(loc->type);
      parameters[2].get_to(loc->source);
      parameters[3].get_to(loc->x);
      parameters[4].get_to(loc->y);
      break;
    }
    case EventCode::Battle_Processing: {
      BattleProcessingCommand* bprocess =
          dynamic_cast<BattleProcessingCommand*>(ret.emplace_back(new BattleProcessingCommand()).get());
      bprocess->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(bprocess->type);
      parameters[1].get_to(bprocess->id);
      parameters[2].get_to(bprocess->canEscape);
      parameters[3].get_to(bprocess->canLose);
      break;
    }
    case EventCode::If_Win: {
      IfWinCommand* win = dynamic_cast<IfWinCommand*>(ret.emplace_back(new IfWinCommand()).get());
      win->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::If_Escape: {
      IfEscapeCommand* escape = dynamic_cast<IfEscapeCommand*>(ret.emplace_back(new IfEscapeCommand()).get());
      escape->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::If_Lose: {
      IfLoseCommand* lose = dynamic_cast<IfLoseCommand*>(ret.emplace_back(new IfLoseCommand()).get());
      lose->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::End_del_Battle_Processing: {
      EndBattleProcessingCommand* end =
          dynamic_cast<EndBattleProcessingCommand*>(ret.emplace_back(new EndBattleProcessingCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Shop_Processing: {
      ShopProcessingCommand* shop =
          dynamic_cast<ShopProcessingCommand*>(ret.emplace_back(new ShopProcessingCommand()).get());
      shop->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(shop->type);
      parameters[1].get_to(shop->id);
      parameters[2].get_to(shop->priceType);
      parameters[3].get_to(shop->price);
      parameters[4].get_to(shop->purchaseOnly);

      while (nextEventCommand() == EventCode::Shop_Processing_Good) {
        ++index;
        ShopProcessingGoodCommand* good =
            dynamic_cast<ShopProcessingGoodCommand*>(shop->goods.emplace_back(new ShopProcessingGoodCommand()).get());
        good->indent = parser[index].value("indent", std::optional<int>{});
        auto params = currentCommand()["parameters"];
        params[0].get_to(good->type);
        params[1].get_to(good->id);
        params[2].get_to(good->priceType);
        params[3].get_to(good->price);
      }
      break;
    }
    case EventCode::Name_Input_Processing: {
      NameInputCommand* shop = dynamic_cast<NameInputCommand*>(ret.emplace_back(new NameInputCommand()).get());
      shop->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(shop->actorId);
      parameters[1].get_to(shop->maxChar);
      break;
    }
    case EventCode::Change_HP: {
      ChangeHPCommand* hp = dynamic_cast<ChangeHPCommand*>(ret.emplace_back(new ChangeHPCommand()).get());
      hp->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(hp->comparison);
      parameters[1].get_to(hp->value);
      parameters[2].get_to(hp->quantityOp);
      parameters[3].get_to(hp->quantitySource);
      parameters[4].get_to(hp->quantity);
      parameters[5].get_to(hp->allowKnockout);
      break;
    }
    case EventCode::Change_MP: {
      ChangeMPCommand* mp = dynamic_cast<ChangeMPCommand*>(ret.emplace_back(new ChangeMPCommand()).get());
      mp->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(mp->comparison);
      parameters[1].get_to(mp->value);
      parameters[2].get_to(mp->quantityOp);
      parameters[3].get_to(mp->quantitySource);
      parameters[4].get_to(mp->quantity);
      break;
    }
    case EventCode::Change_TP: {
      ChangeTPCommand* tp = dynamic_cast<ChangeTPCommand*>(ret.emplace_back(new ChangeTPCommand()).get());
      tp->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(tp->comparison);
      parameters[1].get_to(tp->value);
      parameters[2].get_to(tp->quantityOp);
      parameters[3].get_to(tp->quantitySource);
      parameters[4].get_to(tp->quantity);
      break;
    }
    case EventCode::Change_EXP: {
      ChangeEXPCommand* xp = dynamic_cast<ChangeEXPCommand*>(ret.emplace_back(new ChangeEXPCommand()).get());
      xp->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(xp->comparison);
      parameters[1].get_to(xp->value);
      parameters[2].get_to(xp->quantityOp);
      parameters[3].get_to(xp->quantitySource);
      parameters[4].get_to(xp->quantity);
      parameters[5].get_to(xp->showLevelUp);
      break;
    }
    case EventCode::Change_Level: {
      ChangeLevelCommand* lv = dynamic_cast<ChangeLevelCommand*>(ret.emplace_back(new ChangeLevelCommand()).get());
      lv->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(lv->comparison);
      parameters[1].get_to(lv->value);
      parameters[2].get_to(lv->quantityOp);
      parameters[3].get_to(lv->quantitySource);
      parameters[4].get_to(lv->quantity);
      parameters[5].get_to(lv->showLevelUp);
      break;
    }
    case EventCode::Change_Parameter: {
      ChangeParameterCommand* param =
          dynamic_cast<ChangeParameterCommand*>(ret.emplace_back(new ChangeParameterCommand()).get());
      param->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(param->comparison);
      parameters[1].get_to(param->value);
      parameters[2].get_to(param->param);
      parameters[3].get_to(param->quantityOp);
      parameters[4].get_to(param->quantitySource);
      parameters[5].get_to(param->quantity);
      break;
    }
    case EventCode::Recover_All: {
      RecoverAllCommand* ra = dynamic_cast<RecoverAllCommand*>(ret.emplace_back(new RecoverAllCommand()).get());
      ra->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(ra->comparison);
      parameters[1].get_to(ra->value);
      break;
    }
    case EventCode::Change_Name: {
      ChangeNameCommand* name = dynamic_cast<ChangeNameCommand*>(ret.emplace_back(new ChangeNameCommand()).get());
      name->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(name->actor);
      parameters[1].get_to(name->name);
      break;
    }
    case EventCode::Change_Class: {
      ChangeClassCommand* cc = dynamic_cast<ChangeClassCommand*>(ret.emplace_back(new ChangeClassCommand()).get());
      cc->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(cc->actor);
      parameters[1].get_to(cc->classId);
      parameters[2].get_to(cc->saveLevel);
      break;
    }
    case EventCode::Change_State: {
      ChangeStateCommand* state = dynamic_cast<ChangeStateCommand*>(ret.emplace_back(new ChangeStateCommand()).get());
      state->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(state->comparison);
      parameters[1].get_to(state->value);
      parameters[2].get_to(state->stateOp);
      parameters[3].get_to(state->state);
      break;
    }
    case EventCode::Change_Skill: {
      ChangeSkillCommand* skill = dynamic_cast<ChangeSkillCommand*>(ret.emplace_back(new ChangeSkillCommand()).get());
      skill->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(skill->comparison);
      parameters[1].get_to(skill->value);
      parameters[2].get_to(skill->skillOp);
      parameters[3].get_to(skill->skill);
      break;
    }
    case EventCode::Change_Equipment: {
      ChangeEquipmentCommand* eq =
          dynamic_cast<ChangeEquipmentCommand*>(ret.emplace_back(new ChangeEquipmentCommand()).get());
      eq->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(eq->actorId);
      parameters[1].get_to(eq->equipType);
      parameters[2].get_to(eq->equipment);
      break;
    }
    case EventCode::Change_Profile: {
      ChangeProfileCommand* profile =
          dynamic_cast<ChangeProfileCommand*>(ret.emplace_back(new ChangeProfileCommand()).get());
      profile->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(profile->actor);
      parameters[1].get_to(profile->profile);
      break;
    }
    case EventCode::Change_Nickname: {
      ChangeNicknameCommand* nick =
          dynamic_cast<ChangeNicknameCommand*>(ret.emplace_back(new ChangeNicknameCommand()).get());
      nick->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(nick->actor);
      parameters[1].get_to(nick->nick);
      break;
    }
    case EventCode::Game_Over: {
      GameOverCommand* game = dynamic_cast<GameOverCommand*>(ret.emplace_back(new GameOverCommand()).get());
      game->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Return_To_Title_Screen: {
      ReturnToTitleCommand* game =
          dynamic_cast<ReturnToTitleCommand*>(ret.emplace_back(new ReturnToTitleCommand()).get());
      game->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Open_Menu_Screen: {
      OpenMenuCommand* open = dynamic_cast<OpenMenuCommand*>(ret.emplace_back(new OpenMenuCommand()).get());
      open->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Open_Save_Screen: {
      OpenSaveCommand* open = dynamic_cast<OpenSaveCommand*>(ret.emplace_back(new OpenSaveCommand()).get());
      open->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Abort_Battle: {
      AbortBattleCommand* game = dynamic_cast<AbortBattleCommand*>(ret.emplace_back(new AbortBattleCommand()).get());
      game->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Enemy_Recover_All: {
      EnemyRecoverAllCommand* enemy =
          dynamic_cast<EnemyRecoverAllCommand*>(ret.emplace_back(new EnemyRecoverAllCommand()).get());
      enemy->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(enemy->troop);
      break;
    }
    case EventCode::Enemy_Appear: {
      EnemyAppearCommand* enemy = dynamic_cast<EnemyAppearCommand*>(ret.emplace_back(new EnemyAppearCommand()).get());
      enemy->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(enemy->enemy);
      break;
    }
    case EventCode::Enemy_Transform: {
      EnemyTransformCommand* enemy =
          dynamic_cast<EnemyTransformCommand*>(ret.emplace_back(new EnemyTransformCommand()).get());
      enemy->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(enemy->enemy);
      parameters[1].get_to(enemy->transform);
      break;
    }
    case EventCode::Change_Enemy_HP: {
      ChangeEnemyHPCommand* enemyParam =
          dynamic_cast<ChangeEnemyHPCommand*>(ret.emplace_back(new ChangeEnemyHPCommand()).get());
      enemyParam->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(enemyParam->enemy);
      parameters[1].get_to(enemyParam->enemyOp);
      parameters[2].get_to(enemyParam->quantitySource);
      parameters[3].get_to(enemyParam->quantity);
      parameters[4].get_to(enemyParam->allowKnockOut);
      break;
    }
    case EventCode::Change_Enemy_MP: {
      ChangeEnemyMPCommand* enemyParam =
          dynamic_cast<ChangeEnemyMPCommand*>(ret.emplace_back(new ChangeEnemyMPCommand()).get());
      enemyParam->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(enemyParam->enemy);
      parameters[1].get_to(enemyParam->enemyOp);
      parameters[2].get_to(enemyParam->quantitySource);
      parameters[3].get_to(enemyParam->quantity);
      break;
    }
    case EventCode::Change_Enemy_TP: {
      ChangeEnemyTPCommand* enemyParam =
          dynamic_cast<ChangeEnemyTPCommand*>(ret.emplace_back(new ChangeEnemyTPCommand()).get());
      enemyParam->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(enemyParam->enemy);
      parameters[1].get_to(enemyParam->enemyOp);
      parameters[2].get_to(enemyParam->quantitySource);
      parameters[3].get_to(enemyParam->quantity);
      break;
    }
    case EventCode::Change_Enemy_State: {
      ChangeEnemyStateCommand* enemyParam =
          dynamic_cast<ChangeEnemyStateCommand*>(ret.emplace_back(new ChangeEnemyStateCommand()).get());
      enemyParam->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(enemyParam->enemy);
      parameters[1].get_to(enemyParam->enemyOp);
      parameters[2].get_to(enemyParam->state);
      break;
    }
    case EventCode::Force_Action: {
      ForceActionCommand* action = dynamic_cast<ForceActionCommand*>(ret.emplace_back(new ForceActionCommand()).get());
      action->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(action->sourceComparison);
      parameters[1].get_to(action->source);
      parameters[2].get_to(action->skill);
      parameters[3].get_to(action->target);
      break;
    }
    case EventCode::Show_Battle_Animation: {
      ShowBattleAnimationCommand* battleAnim =
          dynamic_cast<ShowBattleAnimationCommand*>(ret.emplace_back(new ShowBattleAnimationCommand()).get());
      battleAnim->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(battleAnim->enemy);
      parameters[1].get_to(battleAnim->animation);
      parameters[2].get_to(battleAnim->targetAllEnemies);
      break;
    }
    case EventCode::Change_Vehicle_Image: {
      ChangeVehicleImageCommand* vehicle =
          dynamic_cast<ChangeVehicleImageCommand*>(ret.emplace_back(new ChangeVehicleImageCommand()).get());
      vehicle->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(vehicle->vehicle);
      parameters[1].get_to(vehicle->picture);
      parameters[2].get_to(vehicle->pictureIndex);
      break;
    }
    case EventCode::Change_Actor_Images: {
      ChangeActorImageCommand* vehicle =
          dynamic_cast<ChangeActorImageCommand*>(ret.emplace_back(new ChangeActorImageCommand()).get());
      vehicle->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(vehicle->actor);
      parameters[1].get_to(vehicle->charPicture);
      parameters[2].get_to(vehicle->charIndex);
      parameters[3].get_to(vehicle->facePicture);
      parameters[4].get_to(vehicle->faceIndex);
      parameters[5].get_to(vehicle->battlerPicture);
      break;
    }
    case EventCode::Script: {
      ScriptCommand* end = dynamic_cast<ScriptCommand*>(ret.emplace_back(new ScriptCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->script);
      while (nextEventCommand() == EventCode::Next_Script) {
        ++index;
        NextScriptCommand* tmp =
            dynamic_cast<NextScriptCommand*>(end->moreScript.emplace_back(new NextScriptCommand()).get());
        tmp->indent = parser[index].value("indent", std::optional<int>{});
        currentCommand()["parameters"][0].get_to(tmp->script);
      }
      break;
    }
    case EventCode::PluginMV_Command: {
      PluginCommandMV* vehicle = dynamic_cast<PluginCommandMV*>(ret.emplace_back(new PluginCommandMV()).get());
      vehicle->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(vehicle->command);
      break;
    }
    case EventCode::End: {
      EndCommand* end = dynamic_cast<EndCommand*>(ret.emplace_back(new EndCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }

      // MovementRoute commands
    case EventCode::Move_Down: {
      MovementMoveDownCommand* end =
          dynamic_cast<MovementMoveDownCommand*>(ret.emplace_back(new MovementMoveDownCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_Left: {
      MovementMoveLeftCommand* end =
          dynamic_cast<MovementMoveLeftCommand*>(ret.emplace_back(new MovementMoveLeftCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_Right: {
      MovementMoveRightCommand* end =
          dynamic_cast<MovementMoveRightCommand*>(ret.emplace_back(new MovementMoveRightCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_Up: {
      MovementMoveUpCommand* end =
          dynamic_cast<MovementMoveUpCommand*>(ret.emplace_back(new MovementMoveUpCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_Lower_Left: {
      MovementMoveLowerLeftCommand* end =
          dynamic_cast<MovementMoveLowerLeftCommand*>(ret.emplace_back(new MovementMoveLowerLeftCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_Lower_Right: {
      MovementMoveLowerRightCommand* end =
          dynamic_cast<MovementMoveLowerRightCommand*>(ret.emplace_back(new MovementMoveLowerRightCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_Upper_Left: {
      MovementMoveUpperLeftCommand* end =
          dynamic_cast<MovementMoveUpperLeftCommand*>(ret.emplace_back(new MovementMoveUpperLeftCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_Upper_Right: {
      MovementMoveUpperRightCommand* end =
          dynamic_cast<MovementMoveUpperRightCommand*>(ret.emplace_back(new MovementMoveUpperRightCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_at_Random: {
      MovementMoveAtRandomCommand* end =
          dynamic_cast<MovementMoveAtRandomCommand*>(ret.emplace_back(new MovementMoveAtRandomCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_toward_Player: {
      MovementMoveTowardPlayerCommand* end =
          dynamic_cast<MovementMoveTowardPlayerCommand*>(ret.emplace_back(new MovementMoveTowardPlayerCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Move_away_from_Player: {
      MovementMoveAwayFromPlayerCommand* end = dynamic_cast<MovementMoveAwayFromPlayerCommand*>(
          ret.emplace_back(new MovementMoveAwayFromPlayerCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::_1_Step_Forward: {
      MovementMove1StepFowardCommand* end =
          dynamic_cast<MovementMove1StepFowardCommand*>(ret.emplace_back(new MovementMove1StepFowardCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::_1_Step_Backward: {
      MovementMove1StepBackwardCommand* end = dynamic_cast<MovementMove1StepBackwardCommand*>(
          ret.emplace_back(new MovementMove1StepBackwardCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Jump: {
      MovementJumpCommand* end = dynamic_cast<MovementJumpCommand*>(ret.emplace_back(new MovementJumpCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->x);
      parameters[1].get_to(end->y);
      break;
    }
    case EventCode::Wait_del_Movement: {
      MovementWaitCommand* end = dynamic_cast<MovementWaitCommand*>(ret.emplace_back(new MovementWaitCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->duration);
      break;
    }
    case EventCode::Turn_Down: {
      MovementTurnDownCommand* end =
          dynamic_cast<MovementTurnDownCommand*>(ret.emplace_back(new MovementTurnDownCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_Left: {
      MovementTurnLeftCommand* end =
          dynamic_cast<MovementTurnLeftCommand*>(ret.emplace_back(new MovementTurnLeftCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_Right: {
      MovementTurnRightCommand* end =
          dynamic_cast<MovementTurnRightCommand*>(ret.emplace_back(new MovementTurnRightCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_Up: {
      MovementTurnUpCommand* end =
          dynamic_cast<MovementTurnUpCommand*>(ret.emplace_back(new MovementTurnUpCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_90_deg_Right: {
      MovementTurn90DegRightCommand* end =
          dynamic_cast<MovementTurn90DegRightCommand*>(ret.emplace_back(new MovementTurn90DegRightCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_90_deg_Left: {
      MovementTurn90DegLeftCommand* end =
          dynamic_cast<MovementTurn90DegLeftCommand*>(ret.emplace_back(new MovementTurn90DegLeftCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_180_deg: {
      MovementTurn180DegCommand* end =
          dynamic_cast<MovementTurn180DegCommand*>(ret.emplace_back(new MovementTurn180DegCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_90_deg_Left_or_Right: {
      MovementTurn90DegLeftOrRightCommand* end = dynamic_cast<MovementTurn90DegLeftOrRightCommand*>(
          ret.emplace_back(new MovementTurn90DegLeftOrRightCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_at_Random: {
      MovementTurnAtRandomCommand* end =
          dynamic_cast<MovementTurnAtRandomCommand*>(ret.emplace_back(new MovementTurnAtRandomCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_toward_Player: {
      MovementTurnTowardPlayerCommand* end =
          dynamic_cast<MovementTurnTowardPlayerCommand*>(ret.emplace_back(new MovementTurnTowardPlayerCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Turn_away_from_Player: {
      MovementAwayFromPlayerCommand* end =
          dynamic_cast<MovementAwayFromPlayerCommand*>(ret.emplace_back(new MovementAwayFromPlayerCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Switch_ON: {
      MovementSwitchONCommand* end =
          dynamic_cast<MovementSwitchONCommand*>(ret.emplace_back(new MovementSwitchONCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->id);
      break;
    }
    case EventCode::Switch_OFF: {
      MovementSwitchOFFCommand* end =
          dynamic_cast<MovementSwitchOFFCommand*>(ret.emplace_back(new MovementSwitchOFFCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->id);
      break;
    }
    case EventCode::Speed: {
      MovementSpeedCommand* end =
          dynamic_cast<MovementSpeedCommand*>(ret.emplace_back(new MovementSpeedCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->speed);
      break;
    }
    case EventCode::Frequency: {
      MovementFrequencyCommand* end =
          dynamic_cast<MovementFrequencyCommand*>(ret.emplace_back(new MovementFrequencyCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->frequency);
      break;
    }
    case EventCode::Walking_Animation_ON: {
      MovementWalkingAnimationONCommand* end = dynamic_cast<MovementWalkingAnimationONCommand*>(
          ret.emplace_back(new MovementWalkingAnimationONCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Walking_Animation_OFF: {
      MovementWalkingAnimationOFFCommand* end = dynamic_cast<MovementWalkingAnimationOFFCommand*>(
          ret.emplace_back(new MovementWalkingAnimationOFFCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Stepping_Animation_ON: {
      MovementSteppingAnimationONCommand* end = dynamic_cast<MovementSteppingAnimationONCommand*>(
          ret.emplace_back(new MovementSteppingAnimationONCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Stepping_Animation_OFF: {
      MovementSteppingAnimationOFFCommand* end = dynamic_cast<MovementSteppingAnimationOFFCommand*>(
          ret.emplace_back(new MovementSteppingAnimationOFFCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Direction_Fix_ON: {
      MovementDirectionFixONCommand* end =
          dynamic_cast<MovementDirectionFixONCommand*>(ret.emplace_back(new MovementDirectionFixONCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Direction_Fix_OFF: {
      MovementDirectionFixOFFCommand* end =
          dynamic_cast<MovementDirectionFixOFFCommand*>(ret.emplace_back(new MovementDirectionFixOFFCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Through_ON: {
      MovementThroughONCommand* end =
          dynamic_cast<MovementThroughONCommand*>(ret.emplace_back(new MovementThroughONCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Through_OFF: {
      MovementThroughOFFCommand* end =
          dynamic_cast<MovementThroughOFFCommand*>(ret.emplace_back(new MovementThroughOFFCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Transparent_ON: {
      MovementTransparentONCommand* end =
          dynamic_cast<MovementTransparentONCommand*>(ret.emplace_back(new MovementTransparentONCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Transparent_OFF: {
      MovementTransparentOFFCommand* end =
          dynamic_cast<MovementTransparentOFFCommand*>(ret.emplace_back(new MovementTransparentOFFCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Change_Image: {
      MovementChangeImageCommand* end =
          dynamic_cast<MovementChangeImageCommand*>(ret.emplace_back(new MovementChangeImageCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->image);
      parameters[1].get_to(end->character);
      break;
    }
    case EventCode::Change_Opacity: {
      MovementChangeOpacityCommand* end =
          dynamic_cast<MovementChangeOpacityCommand*>(ret.emplace_back(new MovementChangeOpacityCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->opacity);
      break;
    }
    case EventCode::Change_Blend_Mode: {
      MovementChangeBlendModeCommand* end =
          dynamic_cast<MovementChangeBlendModeCommand*>(ret.emplace_back(new MovementChangeBlendModeCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->mode);
      break;
    }
    case EventCode::Play_SE_del_Movement: {
      MovementPlaySECommand* end =
          dynamic_cast<MovementPlaySECommand*>(ret.emplace_back(new MovementPlaySECommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->se);
      break;
    }
    case EventCode::Script_del_Movement: {
      MovementScriptCommand* end =
          dynamic_cast<MovementScriptCommand*>(ret.emplace_back(new MovementScriptCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(end->script);
      break;
    }
    default:
      UnhandledEventCommand* end =
          dynamic_cast<UnhandledEventCommand*>(ret.emplace_back(new UnhandledEventCommand()).get());
      end->indent = parser[index].value("indent", std::optional<int>{});
      end->m_code = code;
      end->data = parser[index];
      end->indent = parser[index].value("indent", std::optional<int>{});
      std::cout << "Unhandled command: " << magic_enum::enum_name(code) << " (" << static_cast<int>(code) << ")"
                << std::endl;
      break;
    }
    ++index;
  }
  return ret;
}

void CommandParser::serialize(nlohmann::json& data, const std::vector<std::shared_ptr<IEventCommand>>& list,
                              bool movementRoute) {
  for (const auto& cmd : list) {
    if (!cmd) {
      continue;
    }
    const auto code = cmd->code();
    bool doParameters = true;
    if ((code >= EventCode::Move_Down && code <= EventCode::Script_del_Movement) || movementRoute) {
      doParameters =
          code == EventCode::Jump || code == EventCode::Change_Blend_Mode || code == EventCode::Change_Image ||
          code == EventCode::Change_Opacity || code == EventCode::Frequency || code == EventCode::Speed ||
          code == EventCode::Play_SE_del_Movement || code == EventCode::Wait_del_Movement ||
          code == EventCode::Script_del_Movement || code == EventCode::Switch_ON || code == EventCode::Switch_OFF;
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
