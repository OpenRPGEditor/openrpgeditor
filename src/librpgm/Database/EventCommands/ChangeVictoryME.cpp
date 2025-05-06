#include "Database/EventCommands/ChangeVictoryME.hpp"

#include "Database/Database.hpp"

ChangeVictoryMECommand::ChangeVictoryMECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(me);
}

void ChangeVictoryMECommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(me); }

std::string ChangeVictoryMECommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Victory ME") + colon.data() + Database::audioText(me) +
         ColorFormatter::popColor(colored);
}
