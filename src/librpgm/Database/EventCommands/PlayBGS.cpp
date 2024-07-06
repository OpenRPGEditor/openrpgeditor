#include "Database/EventCommands/PlayBGS.hpp"

#include "Database/Database.hpp"

PlayBGSCommand::PlayBGSCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(audio);
}

void PlayBGSCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(audio);
}

std::string PlayBGSCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play BGS" + colon.data() +
         db.audioText(audio)+ ColorFormatter::popColor();
}
