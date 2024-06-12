#include "Database/CommandParser.hpp"
#include <iostream>

using json = nlohmann::json;

std::vector<std::shared_ptr<IEventCommand>> CommandParser::parse(const json& _json) {
  std::vector<std::shared_ptr<IEventCommand>> ret;
  parser = _json;

  while (index < parser.size()) {
    EventCode code = EventCode::Event_Dummy;

    parser[index].at("code").get_to(code);
    json parameters = parser[index].value("parameters", json{});
    // std::cout << "Processing: " << magic_enum::enum_name(code) << " (" << static_cast<int>(code) << ")" << std::endl;
    switch (code) {
    case EventCode::Event_Dummy: {
      EventDummy* text = dynamic_cast<EventDummy*>(ret.emplace_back(new EventDummy()).get());
      text->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Show_Text: {
      ShowTextCommand* text = dynamic_cast<ShowTextCommand*>(ret.emplace_back(new ShowTextCommand()).get());
      text->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(text->faceImage);
      parameters[1].get_to(text->faceIndex);
      parameters[2].get_to(text->background);
      parameters[3].get_to(text->position);

      while (nextEventCommand() == EventCode::Next_Text) {
        ++index;
        NextTextCommand* tmp = text->text.emplace_back(new NextTextCommand()).get();
        tmp->indent = parser[index].value("indent", std::optional<int>{});
        currentCommand()["parameters"][0].get_to(tmp->text);
      }
      break;
    }
    case EventCode::Show_Choices: {
      ShowChoiceCommand* choice = dynamic_cast<ShowChoiceCommand*>(ret.emplace_back(new ShowChoiceCommand()).get());
      choice->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(choice->choices);
      parameters[1].get_to(choice->cancelType);
      if (parameters.size() > 2) {
        parameters[2].get_to(choice->defaultType);
      } else {
        choice->defaultType = 0;
      }
      if (parameters.size() > 3) {
        parameters[3].get_to(choice->positionType);
      } else {
        choice->positionType = ChoiceWindowPosition::Right;
      }

      if (parameters.size() > 4) {
        parameters[3].get_to(choice->background);
      } else {
        choice->background = TextBackground::Window;
      }

      if (choice->cancelType >= choice->choices.size()) {
        choice->cancelType = -2;
      }
      break;
    }
    case EventCode::When_Selected: {
      WhenSelectedCommand* selected =
          dynamic_cast<WhenSelectedCommand*>(ret.emplace_back(new WhenSelectedCommand()).get());
      selected->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(selected->param1);
      parameters[1].get_to(selected->choice);
      break;
    }
    case EventCode::When_Cancel: {
      WhenCancelCommand* canceled = dynamic_cast<WhenCancelCommand*>(ret.emplace_back(new WhenCancelCommand()).get());
      canceled->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::End_del_ShowChoices: {
      ShowChoicesEndCommand* canceled =
          dynamic_cast<ShowChoicesEndCommand*>(ret.emplace_back(new ShowChoicesEndCommand()).get());
      canceled->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Input_Number: {
      InputNumberCommand* input = dynamic_cast<InputNumberCommand*>(ret.emplace_back(new InputNumberCommand()).get());
      input->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Select_Item: {
      InputNumberCommand* input = dynamic_cast<InputNumberCommand*>(ret.emplace_back(new InputNumberCommand()).get());
      input->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Show_Scrolling_Text: {
      ShowScrollTextCommand* text =
          dynamic_cast<ShowScrollTextCommand*>(ret.emplace_back(new ShowScrollTextCommand()).get());
      text->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(text->speed);
      parameters[1].get_to(text->noFast);

      while (nextEventCommand() == EventCode::Next_Scrolling_Text) {
        ++index;
        NextScrollingTextCommand* tmp = text->text.emplace_back(new NextScrollingTextCommand()).get();
        tmp->indent = parser[index].value("indent", std::optional<int>{});
        currentCommand()["parameters"][0].get_to(tmp->text);
      }
      break;
    }
    case EventCode::Comment: {
      CommentCommand* text = dynamic_cast<CommentCommand*>(ret.emplace_back(new CommentCommand()).get());
      text->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(text->text);
      while (nextEventCommand() == EventCode::Next_Comment) {
        ++index;
        NextCommentCommand* tmp = text->nextComments.emplace_back(new NextCommentCommand()).get();
        tmp->indent = parser[index].value("indent", std::optional<int>{});
        currentCommand()["parameters"][0].get_to(tmp->text);
      }
      break;
    }
    case EventCode::Conditional_Branch: {
      ConditionalBranchCommand* cond =
          dynamic_cast<ConditionalBranchCommand*>(ret.emplace_back(new ConditionalBranchCommand()).get());
      cond->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(cond->type);
      switch (cond->type) {
      case ConditionType::Switch: {
        parameters[1].get_to(cond->globalSwitch.switchIdx);
        parameters[2].get_to(cond->globalSwitch.checkIfOn);
        break;
      }
      case ConditionType::Variable: {
        parameters[1].get_to(cond->variable.id);
        parameters[2].get_to(cond->variable.source);
        if (cond->variable.source == VariableComparisonSource::Constant) {
          parameters[3].get_to(cond->variable.constant);
        } else {
          parameters[3].get_to(cond->variable.otherId);
        }
        parameters[4].get_to(cond->variable.comparison);
        break;
      }
      case ConditionType::Self_Switch: {
        parameters[1].get_to(cond->selfSw);
        parameters[2].get_to(cond->selfSwitch.checkIfOn);
        break;
      }
      case ConditionType::Timer: {
        parameters[1].get_to(cond->timer.comparison);
        parameters[2].get_to(cond->timer.sec);
        break;
      }
      case ConditionType::Actor: {
        parameters[1].get_to(cond->actor.id);
        parameters[2].get_to(cond->actor.type);
        switch (cond->actor.type) {
        case ActorConditionType::Name:
        case ActorConditionType::In_The_Party:
          if (parameters[3].is_string()) {
            parameters[3].get_to(cond->name);
          }
          break;
        case ActorConditionType::Class:
        case ActorConditionType::Skill:
        case ActorConditionType::Weapon:
        case ActorConditionType::Armor:
        case ActorConditionType::State:
          parameters[3].get_to(cond->actor.checkId);
          break;
        default:
          break;
        }
        break;
      }
      case ConditionType::Enemy: {
        parameters[1].get_to(cond->enemy.id);
        parameters[2].get_to(cond->enemy.type);
        if (cond->enemy.type == EnemyConditionType::State) {
          parameters[3].get_to(cond->enemy.stateId);
        }
        break;
      }
      case ConditionType::Character: {
        parameters[1].get_to(cond->character.id);
        if (cond->character.id > 0) {
          parameters[2].get_to(cond->character.facing);
        }
        break;
      }
      case ConditionType::Gold: {
        parameters[1].get_to(cond->gold.value);
        parameters[2].get_to(cond->gold.type);
        break;
      }
      case ConditionType::Item: {
        parameters[1].get_to(cond->item.id);
        break;
      }
      case ConditionType::Weapon:
      case ConditionType::Armor: {
        parameters[1].get_to(cond->equip.equipId);
        parameters[2].get_to(cond->equip.includeEquipment);
        break;
      }
      case ConditionType::Button: {
        parameters[1].get_to(cond->button);
        break;
      }
      case ConditionType::Script: {
        parameters[1].get_to(cond->script);
        break;
      }
      case ConditionType::Vehicle: {
        parameters[1].get_to(cond->vehicle.id);
        break;
      }
      default:
        break;
      }
      break;
    }
    case EventCode::Else: {
      ElseCommand* c_else = dynamic_cast<ElseCommand*>(ret.emplace_back(new ElseCommand()).get());
      c_else->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Loop: {
      LoopCommand* loop = dynamic_cast<LoopCommand*>(ret.emplace_back(new LoopCommand()).get());
      loop->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Repeat_Above: {
      RepeatAboveCommand* repeat = dynamic_cast<RepeatAboveCommand*>(ret.emplace_back(new RepeatAboveCommand()).get());
      repeat->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Break_Loop: {
      BreakLoopCommand* break_loop = dynamic_cast<BreakLoopCommand*>(ret.emplace_back(new BreakLoopCommand()).get());
      break_loop->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Exit_Event_Processing: {
      ExitEventProecessingCommand* exit =
          dynamic_cast<ExitEventProecessingCommand*>(ret.emplace_back(new ExitEventProecessingCommand()).get());
      exit->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Common_Event: {
      CommonEventCommand* event = dynamic_cast<CommonEventCommand*>(ret.emplace_back(new CommonEventCommand()).get());
      event->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(event->event);
      break;
    }
    case EventCode::Label: {
      LabelCommand* label = dynamic_cast<LabelCommand*>(ret.emplace_back(new LabelCommand()).get());
      label->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(label->label);
      break;
    }
    case EventCode::Jump_To_Label: {
      JumpToLabelCommand* label = dynamic_cast<JumpToLabelCommand*>(ret.emplace_back(new JumpToLabelCommand()).get());
      label->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(label->label);
      break;
    }
    case EventCode::Control_Switches: {
      ControlSwitches* c_switches = dynamic_cast<ControlSwitches*>(ret.emplace_back(new ControlSwitches()).get());
      c_switches->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(c_switches->start);
      parameters[1].get_to(c_switches->end);
      parameters[2].get_to(c_switches->turnOff); // It's inverted because why the fuck not
      break;
    }
    case EventCode::Control_Variables: {
      ControlVariables* variable = dynamic_cast<ControlVariables*>(ret.emplace_back(new ControlVariables()).get());
      variable->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(variable->start);
      parameters[1].get_to(variable->end);
      parameters[2].get_to(variable->operation);
      parameters[3].get_to(variable->operand);
      switch (variable->operand) {
      case VariableControlOperand::Constant:
        parameters[4].get_to(variable->constant);
        break;
      case VariableControlOperand::Variable:
        parameters[4].get_to(variable->variable);
        break;
      case VariableControlOperand::Random:
        parameters[4].get_to(variable->random.min);
        parameters[5].get_to(variable->random.max);
        break;
      case VariableControlOperand::Game_Data:
        parameters[4].get_to(variable->gameData.source);
        parameters[5].get_to(variable->gameData.rawSource);
        parameters[6].get_to(variable->gameData.value);
        break;
      case VariableControlOperand::Script:
        parameters[4].get_to(variable->script);
        break;
      }
      break;
    }
    case EventCode::Control_Self_Switch: {
      ControlSelfSwitch* selfSw = dynamic_cast<ControlSelfSwitch*>(ret.emplace_back(new ControlSelfSwitch()).get());
      selfSw->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(selfSw->selfSw);
      parameters[1].get_to(selfSw->turnOff);
      break;
    }
    case EventCode::Control_Timer: {
      ControlTimer* timer = dynamic_cast<ControlTimer*>(ret.emplace_back(new ControlTimer()).get());
      timer->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(timer->control);
      if (timer->control == TimerControl::Start) {
        parameters[1].get_to(timer->seconds);
      }
      break;
    }
    case EventCode::Change_Gold: {
      ChangeGoldCommmand* gold = dynamic_cast<ChangeGoldCommmand*>(ret.emplace_back(new ChangeGoldCommmand()).get());
      gold->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(gold->operation);
      parameters[1].get_to(gold->operandSource);
      parameters[2].get_to(gold->operand);
      break;
    }
    case EventCode::Change_Items: {
      ChangeItemsCommmand* item = dynamic_cast<ChangeItemsCommmand*>(ret.emplace_back(new ChangeItemsCommmand()).get());
      item->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(item->item);
      parameters[1].get_to(item->operation);
      parameters[2].get_to(item->operandSource);
      parameters[3].get_to(item->operand);
      break;
    }
    case EventCode::Change_Weapons: {
      ChangeWeaponsCommmand* item =
          dynamic_cast<ChangeWeaponsCommmand*>(ret.emplace_back(new ChangeWeaponsCommmand()).get());
      item->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(item->item);
      parameters[1].get_to(item->operation);
      parameters[2].get_to(item->operandSource);
      parameters[3].get_to(item->operand);
      parameters[4].get_to(item->includeEquipment);
      break;
    }
    case EventCode::Change_Armors: {
      ChangeArmorsCommmand* item =
          dynamic_cast<ChangeArmorsCommmand*>(ret.emplace_back(new ChangeArmorsCommmand()).get());
      item->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(item->item);
      parameters[1].get_to(item->operation);
      parameters[2].get_to(item->operandSource);
      parameters[3].get_to(item->operand);
      parameters[4].get_to(item->includeEquipment);
      break;
    }
    case EventCode::Change_Party_Member: {
      ChangePartyMemberCommand* member =
          dynamic_cast<ChangePartyMemberCommand*>(ret.emplace_back(new ChangePartyMemberCommand()).get());
      member->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(member->member);
      parameters[1].get_to(member->operation);
      parameters[2].get_to(member->initialize);
      break;
    }
    case EventCode::Change_Battle_BGM: {
      ChangeBattleBGMCommand* bgm =
          dynamic_cast<ChangeBattleBGMCommand*>(ret.emplace_back(new ChangeBattleBGMCommand()).get());
      bgm->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(bgm->bgm);
      break;
    }
    case EventCode::Change_Victory_ME: {
      ChangeVictoryMECommand* bgm =
          dynamic_cast<ChangeVictoryMECommand*>(ret.emplace_back(new ChangeVictoryMECommand()).get());
      bgm->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(bgm->me);
      break;
    }
    case EventCode::Change_Save_Access: {
      ChangeSaveAccessCommand* save =
          dynamic_cast<ChangeSaveAccessCommand*>(ret.emplace_back(new ChangeSaveAccessCommand()).get());
      save->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(save->access);
      break;
    }
    case EventCode::Change_Menu_Access: {
      ChangeMenuAccessCommand* menu =
          dynamic_cast<ChangeMenuAccessCommand*>(ret.emplace_back(new ChangeMenuAccessCommand()).get());
      menu->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(menu->access);
      break;
    }
    case EventCode::Change_Encounter_Disable: {
      ChangeEncounterDisableCommand* encounter =
          dynamic_cast<ChangeEncounterDisableCommand*>(ret.emplace_back(new ChangeEncounterDisableCommand()).get());
      encounter->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(encounter->access);
      break;
    }
    case EventCode::Change_Formation_Access: {
      ChangeFormationAccessCommand* formation =
          dynamic_cast<ChangeFormationAccessCommand*>(ret.emplace_back(new ChangeFormationAccessCommand()).get());
      formation->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(formation->access);
      break;
    }
    case EventCode::Change_Window_Color: {
      ChangeWindowColorCommand* formation =
          dynamic_cast<ChangeWindowColorCommand*>(ret.emplace_back(new ChangeWindowColorCommand()).get());
      formation->indent = parser[index].value("indent", std::optional<int>{});
      auto colors = parameters[0];
      colors[0].get_to(formation->r);
      colors[1].get_to(formation->g);
      colors[2].get_to(formation->b);
      break;
    }
    case EventCode::Change_Defeat_ME: {
      ChangeDefeatMECommand* bgm =
          dynamic_cast<ChangeDefeatMECommand*>(ret.emplace_back(new ChangeDefeatMECommand()).get());
      bgm->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(bgm->me);
      break;
    }
    case EventCode::Change_Vechicle_BGM: {
      ChangeVehicleBGMCommand* bgm =
          dynamic_cast<ChangeVehicleBGMCommand*>(ret.emplace_back(new ChangeVehicleBGMCommand()).get());
      bgm->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(bgm->me);
      break;
    }
    case EventCode::Transfer_Player: {
      TransferPlayerCommand* transfer =
          dynamic_cast<TransferPlayerCommand*>(ret.emplace_back(new TransferPlayerCommand()).get());
      transfer->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(transfer->mode);
      parameters[1].get_to(transfer->mapId);
      parameters[2].get_to(transfer->x);
      parameters[3].get_to(transfer->y);
      break;
    }
    case EventCode::Set_Vehicle_Location: {
      SetVehicleLocationCommand* transfer =
          dynamic_cast<SetVehicleLocationCommand*>(ret.emplace_back(new SetVehicleLocationCommand()).get());
      transfer->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(transfer->mode);
      parameters[1].get_to(transfer->mapId);
      parameters[2].get_to(transfer->x);
      parameters[3].get_to(transfer->y);
      break;
    }
    case EventCode::Set_Event_Location: {
      SetEventLocationCommand* transfer =
          dynamic_cast<SetEventLocationCommand*>(ret.emplace_back(new SetEventLocationCommand()).get());
      transfer->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(transfer->mode);
      parameters[1].get_to(transfer->mapId);
      parameters[2].get_to(transfer->x);
      parameters[3].get_to(transfer->y);
      break;
    }
    case EventCode::Scroll_Map: {
      ScrollMapCommand* scroll = dynamic_cast<ScrollMapCommand*>(ret.emplace_back(new ScrollMapCommand()).get());
      scroll->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(scroll->direction);
      parameters[1].get_to(scroll->distance);
      parameters[2].get_to(scroll->speed);
      break;
    }
    case EventCode::Set_Movement_Route: {
      SetMovementRouteCommand* route =
          dynamic_cast<SetMovementRouteCommand*>(ret.emplace_back(new SetMovementRouteCommand()).get());
      route->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(route->character);
      parameters[1].get_to(route->route);
      while (nextEventCommand() == EventCode::Movement_Route_Step) {
        ++index;
        MovementRouteStepCommand* tmp = dynamic_cast<MovementRouteStepCommand*>(
            route->editNodes.emplace_back(new MovementRouteStepCommand()).get());
        tmp->indent = parser[index].value("indent", std::optional<int>{});
        CommandParser p;
        tmp->step = p.parse(currentCommand()["parameters"])[0];
      }
      break;
    }

    case EventCode::Get_On_Off_Vehicle: {
      GetOnOffVehicleCommand* step =
          dynamic_cast<GetOnOffVehicleCommand*>(ret.emplace_back(new GetOnOffVehicleCommand()).get());
      step->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Change_Transparency: {
      ChangeTransparencyCommand* step =
          dynamic_cast<ChangeTransparencyCommand*>(ret.emplace_back(new ChangeTransparencyCommand()).get());
      step->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(step->transparency);
      break;
    }
    case EventCode::Show_Animation: {
      ShowAnimationCommand* step =
          dynamic_cast<ShowAnimationCommand*>(ret.emplace_back(new ShowAnimationCommand()).get());
      step->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(step->character);
      parameters[1].get_to(step->animation);
      parameters[2].get_to(step->waitForCompletion);
      break;
    }
    case EventCode::Erase_Event: {
      EraseEventCommand* step = dynamic_cast<EraseEventCommand*>(ret.emplace_back(new EraseEventCommand()).get());
      step->indent = parser[index].value("indent", std::optional<int>{});
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
    case EventCode::Play_SE: {
      PlaySECommand* se = dynamic_cast<PlaySECommand*>(ret.emplace_back(new PlaySECommand()).get());
      se->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(se->audio);
      break;
    }
    case EventCode::Play_ME: {
      PlayMECommand* me = dynamic_cast<PlayMECommand*>(ret.emplace_back(new PlayMECommand()).get());
      me->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(me->audio);
      break;
    }
    case EventCode::Play_BGS: {
      PlayBGSCommand* me = dynamic_cast<PlayBGSCommand*>(ret.emplace_back(new PlayBGSCommand()).get());
      me->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(me->audio);
      break;
    }
    case EventCode::Play_BGM: {
      PlayBGMCommand* me = dynamic_cast<PlayBGMCommand*>(ret.emplace_back(new PlayBGMCommand()).get());
      me->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(me->audio);
      break;
    }
    case EventCode::Play_Move: {
      PlayMovieCommand* mov = dynamic_cast<PlayMovieCommand*>(ret.emplace_back(new PlayMovieCommand()).get());
      mov->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(mov->name);
      break;
    }
    case EventCode::Stop_SE: {
      StopSECommand* se = dynamic_cast<StopSECommand*>(ret.emplace_back(new StopSECommand()).get());
      se->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Resume_BGM: {
      ResumeBGMCommand* se = dynamic_cast<ResumeBGMCommand*>(ret.emplace_back(new ResumeBGMCommand()).get());
      se->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Save_BGM: {
      SaveBGMCommand* se = dynamic_cast<SaveBGMCommand*>(ret.emplace_back(new SaveBGMCommand()).get());
      se->indent = parser[index].value("indent", std::optional<int>{});
      break;
    }
    case EventCode::Fade_Out_BGM: {
      FadeoutBGM* mov = dynamic_cast<FadeoutBGM*>(ret.emplace_back(new FadeoutBGM()).get());
      mov->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(mov->duration);
      break;
    }
    case EventCode::Fade_Out_BGS: {
      FadeoutBGS* mov = dynamic_cast<FadeoutBGS*>(ret.emplace_back(new FadeoutBGS()).get());
      mov->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(mov->duration);
      break;
    }
    case EventCode::Wait: {
      WaitCommand* wait = dynamic_cast<WaitCommand*>(ret.emplace_back(new WaitCommand()).get());
      wait->indent = parser[index].value("indent", std::optional<int>{});
      parameters[0].get_to(wait->duration);
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
      MovementMoveLowerLeftommand* end =
          dynamic_cast<MovementMoveLowerLeftommand*>(ret.emplace_back(new MovementMoveLowerLeftommand()).get());
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
      parameters[0].get_to(end->y);
      break;
    }
    case EventCode::Wait_del_: {
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
      // std::cout << "Unhandled command: " << magic_enum::enum_name(code) << " (" << static_cast<int>(code) << ")" <<
      // std::endl;
      break;
    }
    ++index;
  }
  return ret;
}

void CommandParser::serialize(nlohmann::json& data, const std::vector<std::shared_ptr<IEventCommand>>& list) {}
