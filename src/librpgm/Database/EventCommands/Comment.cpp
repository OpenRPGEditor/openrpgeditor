#include "Database/EventCommands/Comment.hpp"

NextCommentCommand::NextCommentCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(text);
}

void NextCommentCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(text); }

CommentCommand::CommentCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(text);
}

void CommentCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(text); }

std::string CommentCommand::stringRep(const Database& db) const {
  static constexpr std::string_view Name = "Comment";
  std::string ret = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + Name.data() + colon.data() + text + ColorFormatter::popColor();
  for (const auto& t : nextComments) {
    if (!ret.empty()) {
      ret += "\n";
    }
    ret += indentText(indent()) + symbol(t->code()) + ColorFormatter::getColorCode(code()) + std::string(Name.length(), ' ') + colon.data() + t->text + ColorFormatter::popColor();
  }
  return ret;
}