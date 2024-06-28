#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMove1StepBackwardCommand : IEventCommand {
  ~MovementMove1StepBackwardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Backward; }
};
