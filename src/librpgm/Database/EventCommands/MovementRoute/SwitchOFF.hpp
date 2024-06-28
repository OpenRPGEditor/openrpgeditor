#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementSwitchOFFCommand : IEventCommand {
  ~MovementSwitchOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_OFF; }
  int id;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Switch " + std::format("{:04}", id) + " OFF" + ColorFormatter::popColor();
  }
};
