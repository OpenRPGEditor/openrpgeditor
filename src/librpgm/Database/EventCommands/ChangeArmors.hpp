#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeArmorsCommand final : IEventCommand {
  ChangeArmorsCommand() = default;
  explicit ChangeArmorsCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeArmorsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Armors; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  bool includeEquipment;
};