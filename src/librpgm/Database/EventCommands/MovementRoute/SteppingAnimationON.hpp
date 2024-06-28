#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementSteppingAnimationONCommand : IEventCommand {
  ~MovementSteppingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};
