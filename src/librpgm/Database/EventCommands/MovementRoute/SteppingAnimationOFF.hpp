#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementSteppingAnimationOFFCommand : IEventCommand {
  ~MovementSteppingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};
