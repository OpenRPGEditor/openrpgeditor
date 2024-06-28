#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutBGM : IEventCommand {
  ~FadeoutBGM() override = default;
  int duration;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_Out_BGM; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Fadeout BGM :" + std::to_string(duration) + " seconds";
  }
};
