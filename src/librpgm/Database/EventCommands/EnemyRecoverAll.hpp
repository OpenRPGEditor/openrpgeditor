#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyRecoverAllCommand final : IEventCommand {
  EnemyRecoverAllCommand() = default;
  explicit EnemyRecoverAllCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~EnemyRecoverAllCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Recover_All; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  int troop;
};