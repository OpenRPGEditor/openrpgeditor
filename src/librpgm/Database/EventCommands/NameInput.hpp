#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NameInputCommand : IEventCommand {
  ~NameInputCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Name_Input_Processing; }

  int actorId;
  int numChar;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Name Input Processing : {}, " + std::to_string(numChar) + (numChar > 1 ? " character" : " characters" + ColorFormatter::popColor());
  }
};
