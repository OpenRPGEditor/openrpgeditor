#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ControlSelfSwitchCommand final : IEventCommand {
  ControlSelfSwitchCommand() = default;
  explicit ControlSelfSwitchCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ControlSelfSwitchCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  std::string selfSw; // A, B, C, D
  ValueControl turnOff;
};