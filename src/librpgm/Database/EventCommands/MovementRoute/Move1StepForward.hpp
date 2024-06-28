#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementMove1StepFowardCommand : IEventCommand {
  ~MovementMove1StepFowardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Forward; }
};
