#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeParameterCommand final : IEventCommand {
  ChangeParameterCommand() = default;
  explicit ChangeParameterCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeParameterCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Parameter; }

  ActorComparisonSource comparison;
  int value;
  ParameterSource param;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};