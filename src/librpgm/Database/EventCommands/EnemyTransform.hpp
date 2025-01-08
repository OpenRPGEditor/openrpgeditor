#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyTransformCommand final : IEventCommand {
  EnemyTransformCommand() = default;
  explicit EnemyTransformCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~EnemyTransformCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Transform; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<EnemyTransformCommand>(*this); }

  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy;
  int transform;
};