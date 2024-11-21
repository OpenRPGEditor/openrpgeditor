#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowBattleAnimationCommand final : IEventCommand {
  ShowBattleAnimationCommand() = default;
  explicit ShowBattleAnimationCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ShowBattleAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Battle_Animation; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy = 1; // equals 0 if bool is true
  int animation = 1;
  bool targetAllEnemies = false;
};
