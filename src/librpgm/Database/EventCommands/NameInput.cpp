#include "Database/EventCommands/NameInput.hpp"

#include "Database/Database.hpp"

NameInputCommand::NameInputCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(actorId);
  parameters.at(1).get_to(maxChar);
}

void NameInputCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(actorId);
  out.push_back(maxChar);
}

std::string NameInputCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Name Input Processing") + colon.data() + db.actorNameOrId(actorId) + ", " +
         // TL-NOTE(disambiguation): Characters here refers to letters/symbols, e.g: the letter 'A' is a character
         std::to_string(maxChar) + (maxChar > 1 ? trNOOP("characters") : trNOOP("character")) + ColorFormatter::popColor(colored);
}
