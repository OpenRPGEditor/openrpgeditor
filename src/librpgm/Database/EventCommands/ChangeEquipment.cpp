#include "Database/EventCommands/ChangeEquipment.hpp"

#include "Database/Database.hpp"

std::string ChangeEquipmentCommand::stringRep(const Database& db) const {
  const auto actorName = db.actorNameOrId(actorId);
  const auto isDualWield = db.isDualWield(actorId);
  auto eType = equipType;
  if (isDualWield && eType == 2) {
    eType = 1;
  }
  const auto equipTypeName = db.equipTypeName(equipType);
  const auto equipName = equipment == 0 ? "None"
                         : eType <= 1   ? db.weaponNameOrId(equipment)
                                        : db.armorNameOrId(equipment);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Equipment" +
         colon.data() + std::format("{}, {} = {}", actorName, equipTypeName, equipName) + ColorFormatter::popColor();
}
