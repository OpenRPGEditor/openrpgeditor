#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementThroughONCommand : IEventCommand {
  ~MovementThroughONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_ON; }
};
