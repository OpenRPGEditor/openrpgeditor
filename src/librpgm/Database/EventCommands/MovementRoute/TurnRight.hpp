#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurnRightCommand : IEventCommand {
  ~MovementTurnRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Right; }
};