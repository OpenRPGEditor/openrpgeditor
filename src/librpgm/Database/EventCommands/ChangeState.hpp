#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeStateCommand final : IEventCommand {
  ChangeStateCommand() = default;
  explicit ChangeStateCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_State; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeStateCommand>(*this); }

  ActorComparisonSource comparison = ActorComparisonSource::Fixed;
  int value{1};
  PartyMemberOperation stateOp = PartyMemberOperation::_plu__del_Add;
  int state{1};
};