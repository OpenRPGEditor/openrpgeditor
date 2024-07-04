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

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
