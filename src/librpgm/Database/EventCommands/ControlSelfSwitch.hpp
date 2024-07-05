#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ControlSelfSwitchCommand final : IEventCommand {
  ControlSelfSwitchCommand() = default;
  explicit ControlSelfSwitchCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(selfSw);
    parameters[1].get_to(turnOff);
  }
  ~ControlSelfSwitchCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }

  std::string selfSw; // A, B, C, D
  ValueControl turnOff;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};