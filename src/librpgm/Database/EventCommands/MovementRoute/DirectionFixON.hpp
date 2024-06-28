#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementDirectionFixONCommand : IEventCommand {
  ~MovementDirectionFixONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_ON; }
};
