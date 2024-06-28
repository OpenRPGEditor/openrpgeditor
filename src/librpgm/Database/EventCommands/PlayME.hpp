#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlayMECommand : IEventCommand {
  ~PlayMECommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_ME; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Play ME : " + (audio.name == "" ? "None" : audio.name) + " " +
           std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan);
  }
};
