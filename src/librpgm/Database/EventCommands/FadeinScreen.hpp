#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeinScreenCommand : IEventCommand {
  ~FadeinScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadein_Screen; }
};
