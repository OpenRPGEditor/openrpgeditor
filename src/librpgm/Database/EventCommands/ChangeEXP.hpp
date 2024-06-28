#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEXPCommand : IEventCommand {
  ~ChangeEXPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_EXP; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool showLevelUp;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change EXP : {}, " + DecodeEnumName(quantityOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity))
    + (showLevelUp == true ? ColorFormatter::getColor(Color::Gray) + " (Show Level Up)" + ColorFormatter::popColor() : "");
  }
};