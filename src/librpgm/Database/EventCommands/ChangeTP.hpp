#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeTPCommand final : IEventCommand {
  ChangeTPCommand() = default;
  explicit ChangeTPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeTPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_TP; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeTPCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return (quantitySource == QuantityChangeSource::Variable && quantity == targetId) || (comparison == ActorComparisonSource::Variable && value == targetId);
    }
    return false;
  };
  ActorComparisonSource comparison = ActorComparisonSource::Fixed;
  int value{1};
  QuantityChangeOp quantityOp = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource quantitySource = QuantityChangeSource::Constant;
  int quantity{1};
};
