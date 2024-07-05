#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeStateCommand final : IEventCommand {
  ChangeStateCommand() = default;
  explicit ChangeStateCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_State; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison;
  int value;
  PartyMemberOperation stateOp;
  int state;
};