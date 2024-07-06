#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeItemsCommand final : IEventCommand {
  ChangeItemsCommand() = default;
  explicit ChangeItemsCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeItemsCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Items; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
};
