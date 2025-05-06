#include "Database/EventCommands/PlaySE.hpp"

#include "Database/Database.hpp"

PlaySECommand::PlaySECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(audio);
}

void PlaySECommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(audio); }

std::string PlaySECommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Play SE") + colon.data() + db.audioText(audio) + ColorFormatter::popColor(colored);
}
