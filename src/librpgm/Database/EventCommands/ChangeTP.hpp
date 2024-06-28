#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeTPCommand : IEventCommand {
  ~ChangeTPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_TP; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change TP : {}, " + DecodeEnumName(quantityOp) + (quantitySource == QuantityChangeSource::Variable ? " {} " : std::to_string(quantity)) + ColorFormatter::popColor();
  }
};
