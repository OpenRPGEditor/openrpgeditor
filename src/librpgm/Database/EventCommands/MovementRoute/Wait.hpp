#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/EventCommands/Wait.hpp"

#include <format>

struct MovementWaitCommand : WaitCommand {
  ~MovementWaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait_del_Movement; }
};
