#include "Database/EventCommands/PlayBGS.hpp"

#include "Database/Database.hpp"

PlayBGSCommand::PlayBGSCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(bgs);
}

void PlayBGSCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(bgs); }

std::string PlayBGSCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Play BGS") + colon.data() + db.audioText(bgs) + ColorFormatter::popColor(colored);
}
