#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveLeftCommand : IEventCommand {
  ~MovementMoveLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Left; }
};
