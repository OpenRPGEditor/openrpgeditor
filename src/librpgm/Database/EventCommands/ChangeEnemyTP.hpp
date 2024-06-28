#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyTPCommand : IEventCommand {
  ~ChangeEnemyTPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_TP; }
  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Enemy TP : " + (enemy > 0 ? "#" + std::to_string(enemy) + " {}, " : "Entire Troop, ") + DecodeEnumName(enemyOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity))
    + " " + ColorFormatter::popColor();
  }
};
