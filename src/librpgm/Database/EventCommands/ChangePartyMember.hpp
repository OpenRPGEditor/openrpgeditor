#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangePartyMemberCommand : IEventCommand {
  ~ChangePartyMemberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Party_Member; }
  int member;
  PartyMemberOperation operation;
  bool initialize;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Party Member : " + DecodeEnumName(operation) + " {} " + ColorFormatter::popColor() + (initialize == true ? ColorFormatter::getColor(Color::Gray) + "(Initialize)" : "");
  }
};