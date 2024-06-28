#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveRightCommand : IEventCommand {
  ~MovementMoveRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Right; }
};
