#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/Database.hpp"

ChangeNicknameCommand::ChangeNicknameCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
  parameters.at(0).get_to(actor);
  parameters.at(1).get_to(nick);
}

void ChangeNicknameCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(actor);
  out.push_back(nick);
}

std::string ChangeNicknameCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Nickname" + colon.data() + db.actorNameOrId(actor) + ", " + nick + ColorFormatter::popColor();
}
