#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutBGS : IEventCommand {
  ~FadeoutBGS() override = default;
  int duration;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_Out_BGS; }
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Fadeout BGS :" + std::to_string(duration) + " seconds";
  }
};
