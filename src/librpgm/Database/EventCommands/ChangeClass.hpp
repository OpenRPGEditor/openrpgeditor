#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeClassCommand : IEventCommand {
  ~ChangeClassCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Class; }

  int actor;
  int classId;
  bool saveLevel;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Class : {}, {}" + (saveLevel == true ? ColorFormatter::getColor(Color::Gray) + " (Save Level)" + ColorFormatter::popColor() : "");
  }
};