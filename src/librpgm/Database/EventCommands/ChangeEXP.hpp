#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEXPCommand final : IEventCommand {
  ChangeEXPCommand() = default;
  explicit ChangeEXPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEXPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_EXP; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeEXPCommand>(*this); }

  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return (quantitySource == QuantityChangeSource::Variable && quantity == targetId) || (comparison == ActorComparisonSource::Variable && value == targetId);
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (quantitySource == QuantityChangeSource::Variable && quantity == targetId) {
        quantity = newId;
      }
      if (comparison == ActorComparisonSource::Variable && value == targetId) {
        value = newId;
      }
      return true;
    }
    return false;
  }

  ActorComparisonSource comparison = ActorComparisonSource::Fixed;
  int value{1};
  QuantityChangeOp quantityOp = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource quantitySource = QuantityChangeSource::Constant;
  int quantity{1};
  bool showLevelUp{false};
};