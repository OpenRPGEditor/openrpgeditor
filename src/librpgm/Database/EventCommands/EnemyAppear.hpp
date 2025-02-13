#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EnemyAppearCommand final : IEventCommand {
  EnemyAppearCommand() = default;
  explicit EnemyAppearCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~EnemyAppearCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Enemy_Appear; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<EnemyAppearCommand>(*this); }

  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Enemy) {
      return targetId == enemy;
    }
    return false;
  };
  int enemy;
};