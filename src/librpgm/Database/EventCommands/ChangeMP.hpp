#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeMPCommand final : IEventCommand {
  ChangeMPCommand() = default;
  explicit ChangeMPCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeMPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_MP; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;

};
