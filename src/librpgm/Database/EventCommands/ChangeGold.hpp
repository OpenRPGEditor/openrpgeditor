#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeGoldCommmand : IEventCommand {
  ~ChangeGoldCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Gold; }
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
