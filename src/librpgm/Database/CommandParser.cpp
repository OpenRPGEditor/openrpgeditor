#include "Database/CommandParser.hpp"
#include <iostream>

using json = nlohmann::json;

std::vector<std::shared_ptr<IEventCommand>> CommandParser::parse(const json& _json) {
  std::vector<std::shared_ptr<IEventCommand>> ret;
  parser = _json;

  while (index < parser.size()) {
    EventCode code = EventCode::Event_Dummy;

    parser[index].at("code").get_to(code);
    json parameters = parser[index].at("parameters");
    // std::cout << "Processing: " << magic_enum::enum_name(code) << " (" << static_cast<int>(code) << ")" << std::endl;
    switch (code) {
    case EventCode::Event_Dummy: {
      EventDummy* text = dynamic_cast<EventDummy*>(ret.emplace_back(new EventDummy()).get());
      parser[index].at("indent").get_to(text->indent);
      break;
    }
    case EventCode::Show_Text: {
      ShowTextCommand* text = dynamic_cast<ShowTextCommand*>(ret.emplace_back(new ShowTextCommand()).get());
      parser[index].at("indent").get_to(text->indent);
      parameters[0].get_to(text->faceImage);
      parameters[1].get_to(text->faceIndex);
      parameters[2].get_to(text->background);
      parameters[3].get_to(text->position);

      while (nextEventCommand() == EventCode::Next_Text) {
        ++index;
        NextTextCommand* tmp = text->text.emplace_back(new NextTextCommand()).get();
        parser[index].at("indent").get_to(text->indent);
        currentCommand()["parameters"][0].get_to(tmp->text);
      }
      break;
    }
    case EventCode::Show_Choices: {
      ShowChoiceCommand* choice = dynamic_cast<ShowChoiceCommand*>(ret.emplace_back(new ShowChoiceCommand()).get());
      parser[index].at("indent").get_to(choice->indent);
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
      parser[index].at("indent").get_to(selected->indent);
      parameters[0].get_to(selected->param1);
      parameters[1].get_to(selected->choice);
      break;
    }
    case EventCode::When_Cancel: {
      WhenCancelCommand* canceled = dynamic_cast<WhenCancelCommand*>(ret.emplace_back(new WhenCancelCommand()).get());
      parser[index]["indent"].get_to(canceled->indent);
      break;
    }
    case EventCode::Input_Number: {
      InputNumberCommand* input = dynamic_cast<InputNumberCommand*>(ret.emplace_back(new InputNumberCommand()).get());
      parser[index]["indent"].get_to(input->indent);
      break;
    }
    case EventCode::Select_Item: {
      InputNumberCommand* input = dynamic_cast<InputNumberCommand*>(ret.emplace_back(new InputNumberCommand()).get());
      parser[index]["indent"].get_to(input->indent);
      break;
    }
    case EventCode::Show_Scrolling_Text: {
      ShowScrollTextCommand* text =
          dynamic_cast<ShowScrollTextCommand*>(ret.emplace_back(new ShowScrollTextCommand()).get());
      parser[index].at("indent").get_to(text->indent);
      parameters[0].get_to(text->speed);
      parameters[1].get_to(text->noFast);

      while (nextEventCommand() == EventCode::Next_Scrolling_Text) {
        ++index;
        NextScrollingTextCommand* tmp = text->text.emplace_back(new NextScrollingTextCommand()).get();
        parser[index].at("indent").get_to(text->indent);
        currentCommand()["parameters"][0].get_to(tmp->text);
      }
      break;
    }
    case EventCode::Comment: {
      CommentCommand* text = dynamic_cast<CommentCommand*>(ret.emplace_back(new CommentCommand()).get());
      parser[index].at("indent").get_to(text->indent);
      while (nextEventCommand() == EventCode::Next_Comment) {
        ++index;
        NextCommentCommand* tmp = text->text.emplace_back(new NextCommentCommand()).get();
        parser[index].at("indent").get_to(text->indent);
        currentCommand()["parameters"][0].get_to(tmp->text);
      }
      break;
    }
    case EventCode::Conditional_Branch: {
      ConditionalBranchCommand* cond =
          dynamic_cast<ConditionalBranchCommand*>(ret.emplace_back(new ConditionalBranchCommand()).get());
      parser[index].at("indent").get_to(cond->indent);
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
        case ActorConditionType::In_The_Party:
          if (parameters[3].is_string()) {
            parameters[3].get_to(cond->name);
          }
          break;
        case ActorConditionType::Name:
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
        parameters[1].get_to(cond->gold.type);
        parameters[2].get_to(cond->gold.value);
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
      parser[index].at("indent").get_to(c_else->indent);
      break;
    }
    case EventCode::Loop: {
      LoopCommand* loop = dynamic_cast<LoopCommand*>(ret.emplace_back(new LoopCommand()).get());
      parser[index].at("indent").get_to(loop->indent);
      break;
    }
    case EventCode::Repeat_Above: {
      RepeatAboveCommand* repeat = dynamic_cast<RepeatAboveCommand*>(ret.emplace_back(new RepeatAboveCommand()).get());
      parser[index].at("indent").get_to(repeat->indent);
      break;
    }
    case EventCode::Break_Loop: {
      BreakLoopCommand* break_loop = dynamic_cast<BreakLoopCommand*>(ret.emplace_back(new BreakLoopCommand()).get());
      parser[index].at("indent").get_to(break_loop->indent);
      break;
    }
    case EventCode::Exit_Event_Processing: {
      ExitEventProecessingCommand* exit =
          dynamic_cast<ExitEventProecessingCommand*>(ret.emplace_back(new ExitEventProecessingCommand()).get());
      parser[index].at("indent").get_to(exit->indent);
      break;
    }
    case EventCode::Common_Event: {
      CommonEventCommand* event = dynamic_cast<CommonEventCommand*>(ret.emplace_back(new CommonEventCommand()).get());
      parser[index].at("indent").get_to(event->indent);
      parameters[0].get_to(event->event);
      break;
    }
    case EventCode::Label: {
      LabelCommand* label = dynamic_cast<LabelCommand*>(ret.emplace_back(new LabelCommand()).get());
      parser[index].at("indent").get_to(label->indent);
      parameters[0].get_to(label->label);
      break;
    }
    case EventCode::Jump_To_Label: {
      JumpToLabelCommand* label = dynamic_cast<JumpToLabelCommand*>(ret.emplace_back(new JumpToLabelCommand()).get());
      parser[index].at("indent").get_to(label->indent);
      parameters[0].get_to(label->label);
      break;
    }
    case EventCode::Control_Switches: {
      ControlSwitches* c_switches = dynamic_cast<ControlSwitches*>(ret.emplace_back(new ControlSwitches()).get());
      parser[index].at("indent").get_to(c_switches->indent);
      parameters[0].get_to(c_switches->start);
      parameters[1].get_to(c_switches->end);
      parameters[2].get_to(c_switches->turnOff); // It's inverted because why the fuck not
      break;
    }
    case EventCode::Control_Variables: {
      ControlVariables* variable = dynamic_cast<ControlVariables*>(ret.emplace_back(new ControlVariables()).get());
      parser[index].at("indent").get_to(variable->indent);
      parameters[0].get_to(variable->start);
      parameters[1].get_to(variable->end);
      parameters[2].get_to(variable->operation);
      parameters[3].get_to(variable->operand);
      switch (variable->operand) {
      case VariableControlOperand::Constant:
        parameters[2].get_to(variable->constant);
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
      parser[index].at("indent").get_to(selfSw->indent);
      parameters[0].get_to(selfSw->selfSw);
      parameters[1].get_to(selfSw->turnOff);
      break;
    }
    case EventCode::Control_Timer: {
      ControlTimer* timer = dynamic_cast<ControlTimer*>(ret.emplace_back(new ControlTimer()).get());
      parser[index].at("indent").get_to(timer->indent);
      parameters[0].get_to(timer->control);
      if (timer->control == TimerControl::Start) {
        parameters[1].get_to(timer->seconds);
      }
      break;
    }
    case EventCode::Change_Gold: {
      ChangeGoldCommmand* gold = dynamic_cast<ChangeGoldCommmand*>(ret.emplace_back(new ChangeGoldCommmand()).get());
      parser[index].at("indent").get_to(gold->indent);
      parameters[0].get_to(gold->operation);
      parameters[1].get_to(gold->operandSource);
      parameters[2].get_to(gold->operand);
      break;
    }
    case EventCode::Change_Items: {
      ChangeItemsCommmand* item = dynamic_cast<ChangeItemsCommmand*>(ret.emplace_back(new ChangeItemsCommmand()).get());
      parser[index].at("indent").get_to(item->indent);
      parameters[0].get_to(item->item);
      parameters[1].get_to(item->operation);
      parameters[2].get_to(item->operandSource);
      parameters[3].get_to(item->operand);
      break;
    }
    case EventCode::Change_Weapons: {
      ChangeWeaponsCommmand* item =
          dynamic_cast<ChangeWeaponsCommmand*>(ret.emplace_back(new ChangeWeaponsCommmand()).get());
      parser[index].at("indent").get_to(item->indent);
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
      parser[index].at("indent").get_to(item->indent);
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
      parser[index].at("indent").get_to(member->indent);
      parameters[0].get_to(member->member);
      parameters[1].get_to(member->operation);
      parameters[2].get_to(member->initialize);
      break;
    }
    case EventCode::Change_Battle_BGM: {
      ChangeBattleBGMCommand* bgm =
          dynamic_cast<ChangeBattleBGMCommand*>(ret.emplace_back(new ChangeBattleBGMCommand()).get());
      parser[index].at("indent").get_to(bgm->indent);
      parameters[0].get_to(bgm->bgm);
      break;
    }
    case EventCode::Change_Victory_ME: {
      ChangeVictoryMECommand* bgm =
          dynamic_cast<ChangeVictoryMECommand*>(ret.emplace_back(new ChangeVictoryMECommand()).get());
      parser[index].at("indent").get_to(bgm->indent);
      parameters[0].get_to(bgm->me);
      break;
    }
    case EventCode::Change_Save_Access: {
      ChangeSaveAccessCommand* save =
          dynamic_cast<ChangeSaveAccessCommand*>(ret.emplace_back(new ChangeSaveAccessCommand()).get());
      parser[index].at("indent").get_to(save->indent);
      parameters[0].get_to(save->access);
      break;
    }
    case EventCode::Change_Menu_Access: {
      ChangeMenuAccessCommand* menu =
          dynamic_cast<ChangeMenuAccessCommand*>(ret.emplace_back(new ChangeMenuAccessCommand()).get());
      parser[index].at("indent").get_to(menu->indent);
      parameters[0].get_to(menu->access);
      break;
    }
    case EventCode::Change_Encounter_Disable: {
      ChangeEncounterDisableCommand* encounter =
          dynamic_cast<ChangeEncounterDisableCommand*>(ret.emplace_back(new ChangeEncounterDisableCommand()).get());
      parser[index].at("indent").get_to(encounter->indent);
      parameters[0].get_to(encounter->access);
      break;
    }
    case EventCode::Change_Formation_Access: {
      ChangeFormationAccessCommand* formation =
          dynamic_cast<ChangeFormationAccessCommand*>(ret.emplace_back(new ChangeFormationAccessCommand()).get());
      parser[index].at("indent").get_to(formation->indent);
      parameters[0].get_to(formation->access);
      break;
    }
    case EventCode::Change_Window_Color: {
      ChangeWindowColorCommand* formation =
          dynamic_cast<ChangeWindowColorCommand*>(ret.emplace_back(new ChangeWindowColorCommand()).get());
      parser[index].at("indent").get_to(formation->indent);
      parameters[0].get_to(formation->r);
      parameters[1].get_to(formation->g);
      parameters[2].get_to(formation->b);
      break;
    }
    case EventCode::Change_Defeat_ME: {
      ChangeDefeatMECommand* bgm =
          dynamic_cast<ChangeDefeatMECommand*>(ret.emplace_back(new ChangeDefeatMECommand()).get());
      parser[index].at("indent").get_to(bgm->indent);
      parameters[0].get_to(bgm->me);
      break;
    }
    case EventCode::Change_Vechicle_BGM: {
      ChangeVehicleMECommand* bgm =
          dynamic_cast<ChangeVehicleMECommand*>(ret.emplace_back(new ChangeVehicleMECommand()).get());
      parser[index].at("indent").get_to(bgm->indent);
      parameters[0].get_to(bgm->me);
      break;
    }
    case EventCode::Transfer_Player: {
      TransferPlayerCommand* transfer =
          dynamic_cast<TransferPlayerCommand*>(ret.emplace_back(new TransferPlayerCommand()).get());
      parser[index].at("indent").get_to(transfer->indent);
      parameters[0].get_to(transfer->mode);
      parameters[1].get_to(transfer->mapId);
      parameters[2].get_to(transfer->x);
      parameters[3].get_to(transfer->y);
      break;
    }
    case EventCode::Set_Vehicle_Location: {
      SetVehicleLocationCommand* transfer =
          dynamic_cast<SetVehicleLocationCommand*>(ret.emplace_back(new SetVehicleLocationCommand()).get());
      parser[index].at("indent").get_to(transfer->indent);
      parameters[0].get_to(transfer->mode);
      parameters[1].get_to(transfer->mapId);
      parameters[2].get_to(transfer->x);
      parameters[3].get_to(transfer->y);
      break;
    }
    case EventCode::Set_Event_Location: {
      SetVehicleLocationCommand* transfer =
          dynamic_cast<SetVehicleLocationCommand*>(ret.emplace_back(new SetVehicleLocationCommand()).get());
      parser[index].at("indent").get_to(transfer->indent);
      parameters[0].get_to(transfer->mode);
      parameters[1].get_to(transfer->mapId);
      parameters[2].get_to(transfer->x);
      parameters[3].get_to(transfer->y);
      break;
    }
    case EventCode::End: {
      ElseCommand* end = dynamic_cast<ElseCommand*>(ret.emplace_back(new ElseCommand()).get());
      parser[index].at("indent").get_to(end->indent);
      break;
    }
    default:
      // std::cout << "Unhandled command: " << magic_enum::enum_name(code) << " (" << static_cast<int>(code) << ")" <<
      // std::endl;
      break;
    }
    ++index;
  }
  return ret;
}
