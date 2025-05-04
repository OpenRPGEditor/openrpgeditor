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
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  bool hasReference(const int targetId, const SearchType type) override {
    if (type == SearchType::Enemy) {
      return targetId == enemy;
    }
    return false;
  };
  bool setReference(const int targetId, const int newId, const SearchType type) override {
    if (hasReference(targetId, type)) {
      if (type == SearchType::Enemy) {
        enemy = newId;
      }
      return true;
    }
    return false;
  }
  int troopMember{0};
  int enemy{1};
  int m_troopId{0};
};