#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeClassCommand final : IEventCommand {
  ~ChangeClassCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Class; }

  int actor;
  int classId;
  bool saveLevel;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};