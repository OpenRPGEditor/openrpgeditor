#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyStateCommand final : IEventCommand {
  ChangeEnemyStateCommand() = default;
  explicit ChangeEnemyStateCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEnemyStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_State; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeEnemyStateCommand>(*this); }

  int enemy;
  PartyMemberOperation enemyOp;
  int state;
};