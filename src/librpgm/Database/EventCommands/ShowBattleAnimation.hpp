#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowBattleAnimationCommand : IEventCommand {
  ~ShowBattleAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Battle_Animation; }

  int enemy; // equals 0 if bool is true
  int animation;
  bool targetAllEnemies;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Show Battle Animation : "
      + (targetAllEnemies == true ? "Entire Troop, {}" : "#" + std::to_string(enemy) + " {}, {}")
      + ColorFormatter::popColor();
  }
};
