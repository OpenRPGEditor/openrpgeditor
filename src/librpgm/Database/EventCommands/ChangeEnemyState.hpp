#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyStateCommand final : IEventCommand {
  ChangeEnemyStateCommand() = default;
  explicit ChangeEnemyStateCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeEnemyStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_State; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy;
  PartyMemberOperation enemyOp;
  int state;
};