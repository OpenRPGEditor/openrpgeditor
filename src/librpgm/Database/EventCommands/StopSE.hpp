#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct StopSECommand : IEventCommand {
  ~StopSECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stop_SE; }
};
