#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlayBGMCommand : IEventCommand {
  ~PlayBGMCommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGM; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
