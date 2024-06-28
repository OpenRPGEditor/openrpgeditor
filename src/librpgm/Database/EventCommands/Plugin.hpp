#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PluginCommand : IEventCommand {
  ~PluginCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Plugin_Command; }
  std::string command;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Plugin Command : " + command + ColorFormatter::popColor();
  }
};