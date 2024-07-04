#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlSwitches : IEventCommand {
  ~ControlSwitches() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Switches; }
  int start;
  int end;
  ValueControl turnOff;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
