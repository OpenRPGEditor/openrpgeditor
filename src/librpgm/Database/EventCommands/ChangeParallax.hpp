#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeParallaxCommand : IEventCommand {
  ~ChangeParallaxCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Parallax; }

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(image);
    out.push_back(loopHorizontally);
    out.push_back(loopVertically);
    out.push_back(scrollX);
    out.push_back(scrollY);
  }

  std::string image;
  bool loopHorizontally;
  bool loopVertically;
  int scrollX;
  int scrollY;

  std::string stringRep(const Database& db) const override {
    auto rep = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + colon.data() + " " + image +
           ColorFormatter::popColor();
    if (loopHorizontally || loopVertically) {
      rep += ColorFormatter::getColor(Color::Gray) + " (";

      if (loopHorizontally) {
        rep += "Loop Horizontally, ";
      }
      if (loopVertically) {
        rep += "Loop Vertically";
      }
      rep += ")" + ColorFormatter::popColor();
    }

    return rep;
  }
};
