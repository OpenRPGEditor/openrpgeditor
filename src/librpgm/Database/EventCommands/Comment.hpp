#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct NextCommentCommand : IEventCommand {
~NextCommentCommand() override = default;
[[nodiscard]] EventCode code() const override { return EventCode::Next_Comment; }

std::string text;
};
struct CommentCommand : IEventCommand {
  ~CommentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Comment; }
  std::string text;
  std::vector<std::shared_ptr<NextCommentCommand>> nextComments;
  [[nodiscard]] std::string stringRep() const override {
    std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
  + "Comment" + colon.data() + text;
    for (const auto& t : nextComments) {
      if (!ret.empty()) {
        ret += "\n";
      }
      ret += indentText(indent) + colon.data() + indentText(2) + indentText(1) + colon.data() + ColorFormatter::getColorCode(code()) + t->text;
    }
    return ret;
  }
};