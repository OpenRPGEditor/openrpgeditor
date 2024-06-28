#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct MovementPlaySECommand : IEventCommand {
  ~MovementPlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE_del_Movement; }
  Audio se;
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "SE : " + (se.name == "" ? "None" : se.name) + " "
    + std::format("({}, {}, {})", se.volume, se.pitch, se.pan) + ColorFormatter::popColor();
  }
};
