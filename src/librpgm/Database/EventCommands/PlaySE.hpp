#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlaySECommand : IEventCommand {
  ~PlaySECommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Play SE : " + (audio.name == "" ? "None" : audio.name) + " " +
           std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
