#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ResumeBGMCommand : IEventCommand {
  ~ResumeBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Resume_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Resume BGM";
  }
};
