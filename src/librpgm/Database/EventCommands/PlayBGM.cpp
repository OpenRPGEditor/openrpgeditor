#include "Database/EventCommands/PlayBGM.hpp"

#include "Database/Database.hpp"

PlayBGMCommand::PlayBGMCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) { parameters.at(0).get_to(audio); }

void PlayBGMCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(audio); }

std::string PlayBGMCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play BGM" + colon.data() + db.audioText(audio) + ColorFormatter::popColor();
}
