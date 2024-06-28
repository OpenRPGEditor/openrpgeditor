#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeStateCommand : IEventCommand {
  ~ChangeStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_State; }

  ActorComparisonSource comparison;
  int value;
  PartyMemberOperation stateOp;
  int state;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change State : {}, " + DecodeEnumName(stateOp) + " {}" + ColorFormatter::popColor();
  }
};