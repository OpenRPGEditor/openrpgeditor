#include "Database/EventCommands/EnemyTransform.hpp"
#include "Database/Database.hpp"

EnemyTransformCommand::EnemyTransformCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(troopMember);
  parameters.at(1).get_to(enemy);
}

void EnemyTransformCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(troopMember);
  out.push_back(enemy);
}

std::string EnemyTransformCommand::stringRep(const Database& db, const bool colored) const {
  std::string name = db.troopMemberName(m_troopId, troopMember);
  if (troopMember >= 0) {
    name = std::format("#{} {}", troopMember + 1, name);
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Enemy Transform") + colon.data() + " " + name + ", " + db.enemyNameOrId(enemy) +
         ColorFormatter::popColor(colored);
}
