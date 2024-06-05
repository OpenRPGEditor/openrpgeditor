#include "Database/CommandParser.hpp"
#include <iostream>

using json = nlohmann::json;

std::vector<std::shared_ptr<IEventCommand>> CommandParser::parse(const json& data) {
  std::vector<std::shared_ptr<IEventCommand>> ret;
  parser = data;

  while (index < parser.size()) {
    EventCode code = EventCode::Event_Dummy;

    parser[index].at("code").get_to(code);
    std::cout << "Processing: " << magic_enum::enum_name(code) << " (" << static_cast<int>(code) << ")" << std::endl;
    switch(code) {
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
        std::cout << tmp->text << std::endl;
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
      WhenSelectedCommand* selected = dynamic_cast<WhenSelectedCommand*>(ret.emplace_back(new WhenSelectedCommand()).get());
      parser[index].at("indent").get_to(selected->indent);
      auto params = parser[index]["parameters"];
      params[0].get_to(selected->param1);
      params[1].get_to(selected->choice);
      break;
    }
    case EventCode::When_Cancel: {
      WhenCancelCommand* selected = dynamic_cast<WhenCancelCommand*>(ret.emplace_back(new WhenCancelCommand()).get());
      parser[index]["indent"].get_to(selected->indent);
      break;
    }
    default:
      std::cout << "Unhandled command: " << magic_enum::enum_name(code) << " (" << static_cast<int>(code) << ")" << std::endl;
      break;
    }
    ++index;
  }

  return ret;
}
