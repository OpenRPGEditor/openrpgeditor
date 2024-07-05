#include "Database/EventCommands/EnemyRecoverAll.hpp"

#include "Database/Database.hpp"

EnemyRecoverAllCommand::EnemyRecoverAllCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(troop);
}

std::string EnemyRecoverAllCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Enemy Recover All" +
         colon.data() + (troop >= 0 ? "#" + std::to_string(troop + 1) : "Entire Troop") + ColorFormatter::popColor();
}
