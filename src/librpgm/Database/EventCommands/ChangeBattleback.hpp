#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeBattlebackCommand : IEventCommand {
  ~ChangeBattlebackCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_Back; }
  std::string battleBack1Name;
  std::string battleBack2Name;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Battle Back : "
    + battleBack1Name + (battleBack2Name == "" ? "" : " & " + battleBack2Name) + ColorFormatter::popColor();
  }
};