#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ControlSelfSwitchCommand final : IEventCommand {
  ControlSelfSwitchCommand() = default;
  explicit ControlSelfSwitchCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ControlSelfSwitchCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ControlSelfSwitchCommand>(*this); }

  std::string selfSw; // A, B, C, D
  ValueControl turnOff;
};