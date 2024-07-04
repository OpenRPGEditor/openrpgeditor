#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EndCommand : IEventCommand {
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }
};
