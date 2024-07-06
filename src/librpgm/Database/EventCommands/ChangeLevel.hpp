#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeLevelCommand final : IEventCommand {
  ChangeLevelCommand() = default;
  explicit ChangeLevelCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeLevelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Level; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool showLevelUp;
};