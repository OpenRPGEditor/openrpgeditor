#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeItemsCommand final : IEventCommand {
  ChangeItemsCommand() = default;
  explicit ChangeItemsCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(item);
    parameters[1].get_to(operation);
    parameters[2].get_to(operandSource);
    parameters[3].get_to(operand);
  }
  ~ChangeItemsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Items; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
