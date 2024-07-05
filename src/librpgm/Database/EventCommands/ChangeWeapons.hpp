#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeWeaponsCommand final : IEventCommand {
  ChangeWeaponsCommand() = default;
  explicit ChangeWeaponsCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(item);
    parameters[1].get_to(operation);
    parameters[2].get_to(operandSource);
    parameters[3].get_to(operand);
    parameters[4].get_to(includeEquipment);
  }

  ~ChangeWeaponsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Weapons; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  bool includeEquipment;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
