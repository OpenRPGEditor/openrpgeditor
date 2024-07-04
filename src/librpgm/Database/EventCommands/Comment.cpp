#include "Database/EventCommands/Comment.hpp"

std::string CommentCommand::stringRep(const Database& db) const {
  static constexpr std::string_view Name = "Comment";
  std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + Name.data() +
                    colon.data() + text + ColorFormatter::popColor();
  for (const auto& t : nextComments) {
    if (!ret.empty()) {
      ret += "\n";
    }
    ret += indentText(indent) + symbol(t->code()) + ColorFormatter::getColorCode(code()) +
           std::string(Name.length(), ' ') + colon.data() + t->text + ColorFormatter::popColor();
  }
  return ret;
}