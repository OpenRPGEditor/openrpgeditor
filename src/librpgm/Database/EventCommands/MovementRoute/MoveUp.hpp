#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveUpCommand : IEventCommand {
  ~MovementMoveUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Up; }
};
