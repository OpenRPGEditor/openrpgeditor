#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EraseEventCommand : IEventCommand {
  ~EraseEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Event; }
};
