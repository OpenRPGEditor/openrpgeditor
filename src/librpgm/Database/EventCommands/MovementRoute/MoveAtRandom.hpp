#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveAtRandomCommand : IEventCommand {
  ~MovementMoveAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_at_Random; }
};
