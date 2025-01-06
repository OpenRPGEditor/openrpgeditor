#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct CommandParser {
  nlohmann::json parser;
  int index = 0;

  std::vector<std::shared_ptr<IEventCommand>> parse(const nlohmann::ordered_json& data);
  static void serialize(nlohmann::ordered_json& data, const std::vector<std::shared_ptr<IEventCommand>>& list, bool movementRoute = false, bool oldValues = false);

  EventCode nextEventCommand() {
    if (index + 1 >= parser.size()) {
      return EventCode::Event_Dummy;
    }
    EventCode ret;
    parser[index + 1].at("code").get_to(ret);
    return ret;
  }
  nlohmann::json& currentCommand() { return parser[index]; }
};