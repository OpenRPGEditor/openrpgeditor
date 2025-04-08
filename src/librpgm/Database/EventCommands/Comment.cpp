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

std::string CommentCommand::stringRep(const Database& db, const bool colored) const {
  const std::string_view Name = trNOOP("Comment");
  std::string ret = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + Name.data() + colon.data() + text + ColorFormatter::popColor(colored);
  for (const auto& t : nextComments) {
    if (!ret.empty()) {
      ret += "\n";
    }
    ret += indentText(indent()) + symbol(t->code()) + ColorFormatter::getColorCode(code(), colored) + std::string(Name.length(), ' ') + colon.data() + t->text + ColorFormatter::popColor(colored);
  }
  return ret;
}