#include "Database/EventCommands/ChangeEquipment.hpp"
#include "Database/Database.hpp"

ChangeEquipmentCommand::ChangeEquipmentCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(actorId);
  parameters.at(1).get_to(equipType);
  parameters.at(2).get_to(equipment);
}

void ChangeEquipmentCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(actorId);
  out.push_back(equipType);
  out.push_back(equipment);
}

std::string ChangeEquipmentCommand::stringRep(const Database& db, const bool colored) const {
  const auto actorName = db.actorNameOrId(actorId);
  const auto isDualWield = db.isDualWield(actorId);
  auto eType = equipType;
  if (isDualWield && eType == 2) {
    eType = 1;
  }
  const auto equipTypeName = db.equipTypeName(equipType);
  const auto equipName = equipment == 0 ? trNOOP("None") : eType <= 1 ? db.weaponNameOrId(equipment) : db.armorNameOrId(equipment);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Equipment") + colon.data() +
         std::format("{}, {} = {}", actorName, equipTypeName, equipName) + ColorFormatter::popColor(colored);
}
