#include "Database/EventCommands/PlayME.hpp"

#include "Database/Database.hpp"

PlayMECommand::PlayMECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(audio);
}

void PlayMECommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(audio); }

std::string PlayMECommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Play ME") + colon.data() + db.audioText(audio) + ColorFormatter::popColor(colored);
}
