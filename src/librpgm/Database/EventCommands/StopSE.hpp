#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct StopSECommand final : IEventCommand {
  ~StopSECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Stop_SE; }
};
