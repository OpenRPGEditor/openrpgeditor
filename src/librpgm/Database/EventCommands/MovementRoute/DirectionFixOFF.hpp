#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementDirectionFixOFFCommand : IEventCommand {
  ~MovementDirectionFixOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_OFF; }
};
