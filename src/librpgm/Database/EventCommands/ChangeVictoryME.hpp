#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct ChangeVictoryMECommand : IEventCommand {
  ~ChangeVictoryMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Victory_ME; }
  Audio me;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};