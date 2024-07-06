#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct LabelCommand final : IEventCommand {
  LabelCommand() = default;
  explicit LabelCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~LabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Label; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::string label;
};