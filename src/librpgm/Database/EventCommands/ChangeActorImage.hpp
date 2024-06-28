#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeActorImageCommand : IEventCommand {
  ~ChangeActorImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Actor_Images; }

  int actor;
  std::string charPicture;
  int charIndex;
  std::string facePicture;
  int faceIndex;
  std::string battlerPicture;

  [[nodiscard]] std::string stringRep() const override {
    std::string charString = charPicture == "" ? "None, " : charPicture + std::format("({})", charIndex) + " ";
    std::string faceString = facePicture == "" ? "None, " : facePicture + std::format("({})", faceIndex) + " ";
    std::string battlerString = battlerPicture == "" ? "None" : facePicture;

    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Actor Images : {}, " + charPicture + facePicture + battlerPicture
      + ColorFormatter::popColor();
  }
};
