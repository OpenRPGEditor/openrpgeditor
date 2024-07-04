#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct ChangeDefeatMECommand : IEventCommand {
  ~ChangeDefeatMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Defeat_ME; }
  Audio me;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};