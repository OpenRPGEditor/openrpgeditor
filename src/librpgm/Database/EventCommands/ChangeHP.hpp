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

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};