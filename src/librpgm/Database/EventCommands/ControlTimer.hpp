#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ControlTimerCommand final : IEventCommand {
  ControlTimerCommand() = default;
  explicit ControlTimerCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ControlTimerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Timer; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  TimerControl control = TimerControl::Start;
  int seconds = 0;
};
