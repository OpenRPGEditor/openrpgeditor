#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMoveLowerRightCommand : IEventCommand {
  ~MovementMoveLowerRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Right; }
};
