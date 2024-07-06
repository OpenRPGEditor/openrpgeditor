#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeHPCommand final : IEventCommand {
  ChangeHPCommand() = default;
  explicit ChangeHPCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeHPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_HP; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;
  bool allowKnockout;
};