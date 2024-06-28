#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PlayMovieCommand : IEventCommand {
  ~PlayMovieCommand() override = default;
  std::string name;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_Movie; }
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Play Movie : " + (name == "" ? "None" : name);
  }
};
