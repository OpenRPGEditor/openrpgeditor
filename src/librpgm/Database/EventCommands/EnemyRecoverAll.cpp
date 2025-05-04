#include "Database/EventCommands/EnemyRecoverAll.hpp"

#include "Database/Database.hpp"

EnemyRecoverAllCommand::EnemyRecoverAllCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(troopMember);
}

void EnemyRecoverAllCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(troopMember); }

std::string EnemyRecoverAllCommand::stringRep(const Database& db, const bool colored) const {
  std::string name = db.troopMemberName(m_troopId, troopMember);
  if (troopMember >= 0) {
    name = std::format("#{} {}", troopMember + 1, name);
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Enemy Recover All") + colon.data() + name + ColorFormatter::popColor(colored);
}
