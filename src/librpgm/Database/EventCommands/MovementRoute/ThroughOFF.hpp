#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementThroughOFFCommand : IEventCommand {
  ~MovementThroughOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_OFF; }
};
