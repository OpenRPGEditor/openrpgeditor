#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTransparentONCommand : IEventCommand {
  ~MovementTransparentONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_ON; }
};
