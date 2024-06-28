#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeFormationAccessCommand : IEventCommand {
  ~ChangeFormationAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Formation_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Formation Access : " + DecodeEnumName(access) + ColorFormatter::popColor();
  }
};
