#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GameOverCommand final : IEventCommand {
  GameOverCommand() = default;
  explicit GameOverCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~GameOverCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Game_Over; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<GameOverCommand>(*this); }
};