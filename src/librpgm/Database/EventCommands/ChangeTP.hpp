#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeTPCommand final : IEventCommand {
  ChangeTPCommand() = default;
  explicit ChangeTPCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeTPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_TP; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison;
  int value;
  QuantityChangeOp quantityOp;
  QuantityChangeSource quantitySource;
  int quantity;

};
