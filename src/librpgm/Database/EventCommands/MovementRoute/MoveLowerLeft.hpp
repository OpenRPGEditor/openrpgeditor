#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveLowerLeftCommand : IEventCommand {
  ~MovementMoveLowerLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Left; }
};
