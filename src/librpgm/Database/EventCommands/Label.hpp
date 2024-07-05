#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct LabelCommand final : IEventCommand {
  LabelCommand() = default;
  explicit LabelCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(label);
  }
  ~LabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Label; }
  std::string label = "";
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};