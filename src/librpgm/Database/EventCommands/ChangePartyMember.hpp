#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangePartyMemberCommand final : IEventCommand {
  ChangePartyMemberCommand() = default;
  explicit ChangePartyMemberCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangePartyMemberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Party_Member; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangePartyMemberCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Actor) {
      return member == targetId;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (type == SearchType::Actor) {
        member = newId;
      }
      return true;
    }
    return false;
  }
  int member = 1;
  PartyMemberOperation operation = PartyMemberOperation::_plu__del_Add;
  bool initialize = false;
};