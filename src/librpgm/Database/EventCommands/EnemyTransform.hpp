#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyTransformCommand final : IEventCommand {
  EnemyTransformCommand() = default;
  explicit EnemyTransformCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~EnemyTransformCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Transform; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy;
  int transform;
};