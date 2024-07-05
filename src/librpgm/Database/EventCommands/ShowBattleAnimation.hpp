#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowBattleAnimationCommand final : IEventCommand {
  ShowBattleAnimationCommand() = default;
  explicit ShowBattleAnimationCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ShowBattleAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Battle_Animation; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy; // equals 0 if bool is true
  int animation;
  bool targetAllEnemies;
};
