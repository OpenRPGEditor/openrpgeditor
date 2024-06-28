#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeNicknameCommand : IEventCommand {
  ~ChangeNicknameCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Nickname; }

  int actor;
  std::string nick;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Nickname : {}, " + nick
      + ColorFormatter::popColor();
  }
};
