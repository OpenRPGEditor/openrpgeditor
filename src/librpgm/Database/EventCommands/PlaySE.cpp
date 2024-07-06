#include "Database/EventCommands/PlaySE.hpp"

#include "Database/Database.hpp"

PlaySECommand::PlaySECommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(audio);
}

void PlaySECommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(audio);
}


std::string PlaySECommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play SE" + colon.data() +
         db.audioText(audio) + ColorFormatter::popColor();
}
