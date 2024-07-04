#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlayBGSCommand : IEventCommand {
  ~PlayBGSCommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGS; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
