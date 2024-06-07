#include "Database/CommandParser.hpp"
#include <iostream>

using json = nlohmann::json;

std::vector<std::shared_ptr<IEventCommand>> CommandParser::parse(const json& data) {
  std::vector<std::shared_ptr<IEventCommand>> ret;
  parser = data;

  while (index < parser.size()) {
    EventCode code = EventCode::Event_Dummy;

    parser[index].at("code").get_to(code);
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
      json data = parser[index].at("parameters");
      data[0].get_to(text->faceImage);
      data[1].get_to(text->faceIndex);
      data[2].get_to(text->background);
      data[3].get_to(text->position);

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
      auto params = parser[index]["parameters"];
      params[0].get_to(choice->choices);
      params[1].get_to(choice->cancelType);
      if (params.size() > 2) {
        params[2].get_to(choice->defaultType);
      } else {
        choice->defaultType = 0;
      }
      if (params.size() > 3) {
        params[3].get_to(choice->positionType);
      } else {
        choice->positionType = ChoiceWindowPosition::Right;
      }

      if (params.size() > 4) {
        params[3].get_to(choice->background);
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
      auto params = parser[index]["parameters"];
      params[0].get_to(selected->param1);
      params[1].get_to(selected->choice);
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
      json data = parser[index].at("parameters");
      data[0].get_to(text->speed);
      data[1].get_to(text->noFast);

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
      auto params = parser[index]["parameters"];
      cond->type = params[0];
      switch (cond->type) {
      case ConditionType::Switch: {
        cond->globalSwitch.switchIdx = params[1];
        cond->globalSwitch.checkIfOn = params[2];
        break;
      }
      case ConditionType::Variable: {
        cond->variable.id = params[1];
        cond->variable.source = params[2];
        if (cond->variable.source == VariableComparisonSource::Constant) {
          cond->variable.constant = params[3];
        } else {
          cond->variable.otherId = params[3];
        }
        cond->variable.comparison = params[4];
        break;
      }
      case ConditionType::Self_Switch: {
        cond->selfSwitch.id = params[1];
        cond->selfSwitch.checkIfOn = params[2];
        break;
      }
      case ConditionType::Timer: {
        cond->timer.comparison = params[1];
        cond->timer.sec = params[2];
        break;
      }
      case ConditionType::Actor: {
        cond->actor.id = params[1];
        cond->actor.type = params[2];
        switch (cond->actor.type) {
        case ActorConditionType::In_The_Party:
          cond->actor.name = params[3];
          break;
        case ActorConditionType::Name:
        case ActorConditionType::Class:
        case ActorConditionType::Skill:
        case ActorConditionType::Weapon:
        case ActorConditionType::Armor:
        case ActorConditionType::State:
          cond->actor.checkId = params[3];
          break;
        default:
          break;
        }
        break;
      }
      case ConditionType::Enemy: {
        cond->enemy.id = params[1];
        cond->enemy.type = params[2];
        if (cond->enemy.type == EnemyConditionType::State) {
          cond->enemy.stateId = params[3];
        }
        break;
      }
      case ConditionType::Character: {
        cond->character.id = params[1];
        if (cond->character.id > 0) {
          cond->character.facing = params[2];
        }
        break;
      }
      case ConditionType::Gold: {
        cond->gold.type = params[1];
        cond->gold.value = params[2];
        break;
      }
      case ConditionType::Item: {
        cond->item.id = params[1];
        break;
      }
      case ConditionType::Weapon:
      case ConditionType::Armor: {
        cond->equip.equipId = params[1];
        cond->equip.includeEquipment = params[2];
        break;
      }
      case ConditionType::Button: {
        cond->button = params[1];
        break;
      }
      case ConditionType::Script: {
        cond->script = params[1];
        break;
      }
      case ConditionType::Vehicle: {
        cond->vehicle.id = params[1];
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
      event->event = parser[index]["params"][0];
      break;
    }
    case EventCode::Label: {
      LabelCommand* label = dynamic_cast<LabelCommand*>(ret.emplace_back(new LabelCommand()).get());
      parser[index].at("indent").get_to(label->indent);
      label->label = parser[index]["params"][0];
      break;
    }
    case EventCode::Jump_To_Label: {
      JumpToLabelCommand* label = dynamic_cast<JumpToLabelCommand*>(ret.emplace_back(new JumpToLabelCommand()).get());
      parser[index].at("indent").get_to(label->indent);
      label->label = parser[index]["params"][0];
      break;
    }
    case EventCode::Control_Switches: {
      ControlSwitches* c_switches = dynamic_cast<ControlSwitches*>(ret.emplace_back(new ControlSwitches()).get());
      parser[index].at("indent").get_to(c_switches->indent);
      c_switches->start = parser[index]["params"][0];
      c_switches->end = parser[index]["params"][1];
      c_switches->turnOff = parser[index]["params"][2]; // It's inverted because why the fuck not
      break;
    }
    case EventCode::Control_Variables: {
      ControlVariables* variable = dynamic_cast<ControlVariables*>(ret.emplace_back(new ControlVariables()).get());
      parser[index].at("indent").get_to(variable->indent);
      variable->start = parser[index]["params"][0];
      variable->end = parser[index]["params"][1];
      variable->operation = parser[index]["params"][2];
      variable->operand = parser[index]["params"][3];
      switch (variable->operand) {
      case VariableControlOperand::Constant:
        variable->constant = parser[index]["params"][4];
        break;
      case VariableControlOperand::Variable:
        variable->variable = parser[index]["params"][4];
        break;
      case VariableControlOperand::Random:
        variable->random.min = parser[index]["params"][4];
        variable->random.min = parser[index]["params"][5];
        break;
      case VariableControlOperand::Game_Data:
        variable->gameData.source = parser[index]["params"][4];
        variable->gameData.rawSource = parser[index]["params"][5];
        variable->gameData.value = parser[index]["params"][6];
        break;
      case VariableControlOperand::Script:
        variable->script = parser[index]["params"][4];
        ;
        break;
      }
      break;
    }
    case EventCode::Control_Self_Switch: {
      ControlSelfSwitch* selfSw = dynamic_cast<ControlSelfSwitch*>(ret.emplace_back(new ControlSelfSwitch()).get());
      parser[index].at("indent").get_to(selfSw->indent);
      selfSw->selfSw = parser[index]["params"][0];
      selfSw->turnOff = parser[index]["params"][1];
      break;
    }
    case EventCode::Control_Timer: {
      ControlTimer* timer = dynamic_cast<ControlTimer*>(ret.emplace_back(new ControlTimer()).get());
      parser[index].at("indent").get_to(timer->indent);
      timer->control = parser[index]["parameters"][0];
      if (timer->control == TimerControl::Start) {
        timer->seconds = parser[index]["parameters"][1];
      }
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
