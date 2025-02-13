#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeWeaponsCommand final : IEventCommand {
  ChangeWeaponsCommand() = default;
  explicit ChangeWeaponsCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);

  ~ChangeWeaponsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Weapons; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeWeaponsCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return operandSource == QuantityChangeSource::Variable && operand == targetId;
    }
    if (type == SearchType::Weapons) {
      return targetId == item;
    }
    return false;
  };
  int item{1};
  QuantityChangeOp operation = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource operandSource = QuantityChangeSource::Constant;
  int operand{1};
  bool includeEquipment{false};
};
