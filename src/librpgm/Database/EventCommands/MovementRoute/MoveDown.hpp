#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveDownCommand : IEventCommand {
  ~MovementMoveDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Down; }
};
