#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangePlayerFollowersCommand : IEventCommand {
  ~ChangePlayerFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Player_Followers; }

  bool followersEnabled;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Player Followers : " + (followersEnabled == true ? "ON" : "OFF")
      + ColorFormatter::popColor();
  }
};
