#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyRecoverAllCommand : IEventCommand {
  ~EnemyRecoverAllCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Recover_All; }
  int troop;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Enemy Recover All : " + (troop > 0 ? "#" + std::to_string(troop) + " {}" : "Entire Troop") + ColorFormatter::popColor();
  }
};