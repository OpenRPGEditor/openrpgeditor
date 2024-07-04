#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeLevelCommand : IEventCommand {
  ~ChangeLevelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Level; }

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool showLevelUp;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};