#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct NextScrollingTextCommand : IEventCommand {
  ~NextScrollingTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Scrolling_Text; }

  std::string text;
};

struct ShowScrollTextCommand : IEventCommand {
  ~ShowScrollTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Scrolling_Text; }

  int speed;
  bool noFast;
  std::vector<std::shared_ptr<NextScrollingTextCommand>> text;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
  + "Text(S) " + colon.data() + " Speed " + std::to_string(speed) + (noFast == true ? ", No Fast Forward" : "");

    for (const auto& t : text) {
      if (!ret.empty()) {
        ret += "\n";
      }
      ret += indentText(indent) + colon.data() + indentText(1) + indentText(1) + colon.data() + ColorFormatter::getColorCode(code()) + t->text;
    }
    return ret;
  }
};