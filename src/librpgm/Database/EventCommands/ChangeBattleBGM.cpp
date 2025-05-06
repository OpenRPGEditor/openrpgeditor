#include "Database/EventCommands/ChangeBattleBGM.hpp"

#include "Database/Database.hpp"

ChangeBattleBGMCommand::ChangeBattleBGMCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(bgm);
}

void ChangeBattleBGMCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(bgm); }

std::string ChangeBattleBGMCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Battle BGM") + colon.data() + db.audioText(bgm) + ColorFormatter::popColor(colored);
}
