#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeArmorsCommand final : IEventCommand {
  ChangeArmorsCommand() = default;
  explicit ChangeArmorsCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(item);
    parameters[1].get_to(operation);
    parameters[2].get_to(operandSource);
    parameters[3].get_to(operand);
    parameters[4].get_to(includeEquipment);
  }
  ~ChangeArmorsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Armors; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  bool includeEquipment;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};