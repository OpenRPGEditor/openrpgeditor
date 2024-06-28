#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementWalkingAnimationOFFCommand : IEventCommand {
  ~MovementWalkingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};
