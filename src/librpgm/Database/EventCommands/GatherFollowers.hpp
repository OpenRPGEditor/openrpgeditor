#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GatherFollowersCommand : IEventCommand {
  ~GatherFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Gather_Followers; }
};
