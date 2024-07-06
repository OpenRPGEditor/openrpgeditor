#include "Database/EventCommands/PlayME.hpp"

#include "Database/Database.hpp"

PlayMECommand::PlayMECommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(audio);
}

void PlayMECommand::serializeParameters(nlohmann::json& out) const { out.push_back(audio); }

std::string PlayMECommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play ME" + colon.data() +
         db.audioText(audio) + ColorFormatter::popColor();
}
