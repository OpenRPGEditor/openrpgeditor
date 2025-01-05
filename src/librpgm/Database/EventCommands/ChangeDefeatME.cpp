#include "Database/EventCommands/ChangeDefeatME.hpp"

#include "Database/Database.hpp"

ChangeDefeatMECommand::ChangeDefeatMECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) { parameters.at(0).get_to(me); }

void ChangeDefeatMECommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(me); }

std::string ChangeDefeatMECommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Defeat ME" + colon.data() + db.audioText(me) + ColorFormatter::popColor();
}
