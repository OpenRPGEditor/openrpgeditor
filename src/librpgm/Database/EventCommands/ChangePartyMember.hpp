#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangePartyMemberCommand final : IEventCommand {
  ChangePartyMemberCommand() = default;
  explicit ChangePartyMemberCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangePartyMemberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Party_Member; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int member = 1;
  PartyMemberOperation operation = PartyMemberOperation::_plu__del_Add;
  bool initialize = false;
};