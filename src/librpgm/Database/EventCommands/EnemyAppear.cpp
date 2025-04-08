#include "Database/EventCommands/EnemyAppear.hpp"

EnemyAppearCommand::EnemyAppearCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(enemy);
}

void EnemyAppearCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(enemy); }

std::string EnemyAppearCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Enemy Appear") + colon.data() + "#" + std::to_string(enemy + 1) +
         ColorFormatter::popColor(colored);
}
