#include "Database/EventCommands/SetEventLocation.hpp"
#include "Database/Database.hpp"

std::string SetEventLocationCommand::stringRep(const Database& db) const {
  const auto map = db.mapInfos.currentMap();
  const auto ev = map ? map->event(event) : nullptr;
  const auto evName = ev && !ev->name.empty() ? ev->name : std::format("#{:03}", event);
  const auto prefix = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) +
                      "Set Event Location" + colon.data() + (event > 0 ? evName : "This Event");
  const auto suffix = " (Direction: " + DecodeEnumName(direction) + ")" + ColorFormatter::popColor();

  if (mode == TransferMode::Variable_Designation) {
    auto varX = db.system.variable(x);
    auto varY = db.system.variable(y);
    varX = varX.empty() ? std::format("#{:04}", x) : varX;
    varY = varY.empty() ? std::format("#{:04}", y) : varY;
    return prefix + std::format(", ({{{}}},{{{}}})", varX, varY) + suffix;
  }

  if (mode == TransferMode::Exchange_With_Another_Event) {
    return prefix + ", Exchange with " + (event > 0 ? evName : "This Event") + suffix;
  }

  return prefix + std::format(", ({}, {})", x, y) + suffix;
}
