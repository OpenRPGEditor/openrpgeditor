#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyStateCommand : IEventCommand {
  ~ChangeEnemyStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_TP; }
  int enemy;
  PartyMemberOperation enemyOp;
  int state;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Enemy State : " + (enemy > 0 ? "#" + std::to_string(enemy) + " {}, " : "Entire Troop, ") + DecodeEnumName(enemyOp) + "{} "
      + ColorFormatter::popColor();
  }
};