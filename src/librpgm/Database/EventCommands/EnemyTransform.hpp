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
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Enemy) {
      return targetId == enemy;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (type == SearchType::Enemy) {
        enemy = newId;
      }
      return true;
    }
    return false;
  }
  int enemy;
  int transform;
};