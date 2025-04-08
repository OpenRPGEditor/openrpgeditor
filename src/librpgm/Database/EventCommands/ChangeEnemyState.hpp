#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyStateCommand final : IEventCommand {
  ChangeEnemyStateCommand() = default;
  explicit ChangeEnemyStateCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEnemyStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_State; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;

  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeEnemyStateCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Enemy) {
      return enemy == targetId;
    }
    if (type == SearchType::State) {
      return targetId == state;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (type == SearchType::Enemy) {
        enemy = newId;
      }
      if (type == SearchType::State) {
        state = newId;
      }

      return true;
    }
    return false;
  }
  int enemy;
  PartyMemberOperation enemyOp;
  int state;
};