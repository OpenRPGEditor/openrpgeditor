#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEXPCommand final : IEventCommand {
  ~ChangeEXPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_EXP; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool showLevelUp;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};