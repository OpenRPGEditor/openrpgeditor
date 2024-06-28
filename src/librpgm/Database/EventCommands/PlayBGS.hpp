#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlayBGSCommand : IEventCommand {
  ~PlayBGSCommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGS; }
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Play BGS : " + (audio.name == "" ? "None" : audio.name) +
           " " + std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
