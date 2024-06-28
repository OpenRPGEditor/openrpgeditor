#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveTowardPlayerCommand : IEventCommand {
  ~MovementMoveTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_toward_Player; }
};
