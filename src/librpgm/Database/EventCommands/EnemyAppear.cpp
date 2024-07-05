#include "Database/EventCommands/EnemyAppear.hpp"

EnemyAppearCommand::EnemyAppearCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(enemy);
}

std::string EnemyAppearCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Enemy Appear" + colon.data() +
         "#" + std::to_string(enemy + 1) + ColorFormatter::popColor();
}
