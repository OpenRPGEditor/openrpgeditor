#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GatherFollowersCommand : IEventCommand {
  ~GatherFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Gather_Followers; }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Gather Followers"
      + ColorFormatter::popColor();
  }
};
