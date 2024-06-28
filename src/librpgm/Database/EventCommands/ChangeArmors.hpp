#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeArmorsCommmand : IEventCommand {
  ~ChangeArmorsCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Armors; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  bool includeEquipment;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Armors : {} " + DecodeEnumName(operation) + (operandSource == QuantityChangeSource::Constant ? std::to_string(operand) : " [] ") + ColorFormatter::popColor() + (includeEquipment == true ? ColorFormatter::getColor(Color::Gray) + "(Include Equipment)" : "");
  }
};