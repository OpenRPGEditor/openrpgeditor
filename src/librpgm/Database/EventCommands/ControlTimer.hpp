#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ControlTimer : IEventCommand {
  ~ControlTimer() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Timer; }
  TimerControl control;
  int seconds;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
