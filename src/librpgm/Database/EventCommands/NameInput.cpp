#include "Database/EventCommands/NameInput.hpp"

#include "Database/Database.hpp"

NameInputCommand::NameInputCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(actorId);
  parameters[1].get_to(maxChar);
}

void NameInputCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(actorId);
  out.push_back(maxChar);
}

std::string NameInputCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Name Input Processing" +
         colon.data() + db.actorNameOrId(actorId) + ", " + std::to_string(maxChar) +
         (maxChar > 1 ? " characters" : " character") + ColorFormatter::popColor();
}
