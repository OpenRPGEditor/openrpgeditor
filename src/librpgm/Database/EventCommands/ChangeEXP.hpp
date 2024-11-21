#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEXPCommand final : IEventCommand {
  ChangeEXPCommand() = default;
  explicit ChangeEXPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEXPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_EXP; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison = ActorComparisonSource::Fixed;
  int value{1};
  QuantityChangeOp quantityOp = QuantityChangeOp::_plu__del_Increase;
  QuantityChangeSource quantitySource = QuantityChangeSource::Constant;
  int quantity{1};
  bool showLevelUp{false};

};