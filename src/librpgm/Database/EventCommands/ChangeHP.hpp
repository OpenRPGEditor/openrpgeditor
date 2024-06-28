#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeHPCommand : IEventCommand {
  ~ChangeHPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_HP; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool allowKnockout;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change HP : {}, " + DecodeEnumName(quantityOp) + (quantitySource == QuantityChangeSource::Variable ? " [] " : std::to_string(quantity))
    + (allowKnockout == true ? ColorFormatter::getColor(Color::Gray) + " (Allow Knockout)" + ColorFormatter::popColor() : "");
  }
};