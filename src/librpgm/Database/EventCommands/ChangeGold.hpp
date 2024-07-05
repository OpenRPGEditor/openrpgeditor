#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeGoldCommand final : IEventCommand {
  ChangeGoldCommand() = default;
  explicit ChangeGoldCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(operation);
    parameters[1].get_to(operandSource);
    parameters[2].get_to(operand);
  }
  ~ChangeGoldCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Gold; }
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
