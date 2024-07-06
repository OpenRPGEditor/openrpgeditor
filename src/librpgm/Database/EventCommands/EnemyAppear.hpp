#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyAppearCommand final : IEventCommand {
  EnemyAppearCommand() = default;
  explicit EnemyAppearCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~EnemyAppearCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Appear; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy;
};