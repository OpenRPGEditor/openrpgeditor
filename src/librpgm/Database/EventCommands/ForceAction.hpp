#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ForceActionCommand : IEventCommand {
  ~ForceActionCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Force_Action; }

  SubjectComparisonSource sourceComparison;
  int source;
  int skill;
  int target;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Force Action : "
      + (sourceComparison == SubjectComparisonSource::Enemy ? ("#" + std::to_string(source) + " {},") : "{},")
      + " {},"
      + (target == -2 ? "Last Target" : target == -1 ? "Random" : "Index " + std::to_string(target))
      + ColorFormatter::popColor();
  }
};
