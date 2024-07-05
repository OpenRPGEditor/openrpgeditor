#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GameOverCommand final : IEventCommand {
  ~GameOverCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Game_Over; }
};