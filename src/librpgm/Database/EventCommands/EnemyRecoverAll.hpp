#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyRecoverAllCommand final : IEventCommand {
  EnemyRecoverAllCommand() = default;
  explicit EnemyRecoverAllCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~EnemyRecoverAllCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Recover_All; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int troop;
};