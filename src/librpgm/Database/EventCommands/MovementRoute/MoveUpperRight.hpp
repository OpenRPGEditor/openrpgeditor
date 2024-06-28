#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveUpperRightCommand : IEventCommand {
  ~MovementMoveUpperRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Right; }
};