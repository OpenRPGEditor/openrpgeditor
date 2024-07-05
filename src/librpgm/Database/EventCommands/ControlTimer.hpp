#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ControlTimerCommand : IEventCommand {
  ControlTimerCommand() = default;
  explicit ControlTimerCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ControlTimerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Timer; }
  TimerControl control = TimerControl::Start;
  int seconds = 0;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
