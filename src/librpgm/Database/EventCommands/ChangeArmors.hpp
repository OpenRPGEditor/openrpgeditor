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
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};