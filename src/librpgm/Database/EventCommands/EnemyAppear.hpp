#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyAppearCommand : IEventCommand {
  ~EnemyAppearCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Appear; }
  int enemy;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Enemy Appear : #" + std::to_string(enemy) + " {}" + ColorFormatter::popColor();
  }
};