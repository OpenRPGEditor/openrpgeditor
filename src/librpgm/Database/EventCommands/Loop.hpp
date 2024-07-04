#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct LoopCommand : IEventCommand {
  ~LoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Loop; }
};
