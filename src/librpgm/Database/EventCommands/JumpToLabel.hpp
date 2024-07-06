#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct JumpToLabelCommand final : IEventCommand {
  JumpToLabelCommand() = default;
  explicit JumpToLabelCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~JumpToLabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump_To_Label; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::string label;
};