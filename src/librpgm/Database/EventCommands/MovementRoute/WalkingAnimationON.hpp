#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementWalkingAnimationONCommand : IEventCommand {
  ~MovementWalkingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};
