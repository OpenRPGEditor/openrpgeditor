#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementTransparentOFFCommand : IEventCommand {
  ~MovementTransparentOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_OFF; }
};