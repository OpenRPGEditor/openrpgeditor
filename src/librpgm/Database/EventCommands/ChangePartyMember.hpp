#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangePartyMemberCommand final : IEventCommand {
  ChangePartyMemberCommand() = default;
  explicit ChangePartyMemberCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(member);
    parameters[1].get_to(operation);
    parameters[2].get_to(initialize);
  }
  ~ChangePartyMemberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Party_Member; }
  int member;
  PartyMemberOperation operation;
  bool initialize;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};