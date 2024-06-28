#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyHPCommand : IEventCommand {
  ~ChangeEnemyHPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_HP; }
  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool allowKnockOut;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Enemy HP : " + (enemy > 0 ? "#" + std::to_string(enemy) + " {}, " : "Entire Troop, ") + DecodeEnumName(enemyOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity))
    + " " + (allowKnockOut == true ? ColorFormatter::getColor(Color::Gray) + " (Allow Knockout)" + ColorFormatter::popColor() : "");
  }
};