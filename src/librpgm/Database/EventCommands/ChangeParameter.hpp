#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeParameterCommand final : IEventCommand {
  ChangeParameterCommand() = default;
  explicit ChangeParameterCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeParameterCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Parameter; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison;
  int value;
  ParameterSource param;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;

};