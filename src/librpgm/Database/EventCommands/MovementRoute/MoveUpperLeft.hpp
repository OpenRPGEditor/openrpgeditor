#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveUpperLeftCommand : IEventCommand {
  ~MovementMoveUpperLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Left; }
};
