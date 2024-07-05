#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct JumpToLabelCommand final : IEventCommand {
  JumpToLabelCommand() = default;
  explicit JumpToLabelCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(label);
  }
  ~JumpToLabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump_To_Label; }
  std::string label = "";
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};