#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangePartyMemberCommand final : IEventCommand {
  ChangePartyMemberCommand() = default;
  explicit ChangePartyMemberCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangePartyMemberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Party_Member; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangePartyMemberCommand>(*this); }

  int member = 1;
  PartyMemberOperation operation = PartyMemberOperation::_plu__del_Add;
  bool initialize = false;
};