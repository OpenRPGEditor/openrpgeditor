#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutScreenCommand : IEventCommand {
  ~FadeoutScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadeout_Screen; }
};
