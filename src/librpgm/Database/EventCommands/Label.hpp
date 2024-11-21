#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct LabelCommand final : IEventCommand {
  LabelCommand() = default;
  explicit LabelCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~LabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Label; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::string label;
};