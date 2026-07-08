#include "Database/EventCommands/PlayBGM.hpp"

#include "Database/Database.hpp"

PlayBGMCommand::PlayBGMCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(bgm);
}

void PlayBGMCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(bgm); }

std::string PlayBGMCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Play BGM") + colon.data() + db.audioText(bgm) + ColorFormatter::popColor(colored);
}
