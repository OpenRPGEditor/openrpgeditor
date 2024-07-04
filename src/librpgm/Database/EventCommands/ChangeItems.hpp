#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeItemsCommmand : IEventCommand {
  ~ChangeItemsCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Items; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

