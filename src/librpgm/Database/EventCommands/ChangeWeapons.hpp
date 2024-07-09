#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeWeaponsCommand final : IEventCommand {
  ChangeWeaponsCommand() = default;
  explicit ChangeWeaponsCommand(const std::optional<int>& indent, const nlohmann::json& parameters);

  ~ChangeWeaponsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Weapons; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int item{1};
  QuantityChangeOp operation = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource operandSource = QuantityChangeSource::Constant;
  int operand{1};
  bool includeEquipment{false};
};
