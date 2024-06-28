#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTurnAtRandomCommand : IEventCommand {
  ~MovementTurnAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_at_Random; }
};