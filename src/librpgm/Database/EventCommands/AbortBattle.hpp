#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct AbortBattleCommand : IEventCommand {
  ~AbortBattleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Abort_Battle; }
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Abort Battle" + ColorFormatter::popColor();
  }
};