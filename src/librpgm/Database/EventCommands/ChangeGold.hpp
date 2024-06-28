#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeGoldCommmand : IEventCommand {
  ~ChangeGoldCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Gold; }
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  [[nodiscard]] std::string stringRep() const override {
    if (operandSource == QuantityChangeSource::Constant)
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Gold: " + (operation == QuantityChangeOp::_plu__del_Increase ? "+ " : "- ") + std::to_string(operand);

    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Gold: " + (operation == QuantityChangeOp::_plu__del_Increase ? "+ " : "- "); // Add variable name at the end
  }
};
