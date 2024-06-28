#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct ChangeBattleBGMCommand : IEventCommand {
  ~ChangeBattleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_BGM; }
  Audio bgm;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Battle BGM : " + (bgm.name == "" ? "None" : bgm.name) +
           " " + std::format("({}, {}, {})", bgm.volume, bgm.pitch, bgm.pan) + ColorFormatter::popColor();
  }
};