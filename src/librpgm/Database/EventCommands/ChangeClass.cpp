#include "Database/EventCommands/ChangeClass.hpp"

#include "Database/Database.hpp"

std::string ChangeClassCommand::stringRep(const Database& db) const {
  const auto actName = db.nameOrId(db.actorName(actor), actor);
  const auto clsName = db.nameOrId(db.className(classId), classId);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Class" + colon.data() +
         actName + ", " + clsName + ", " + (saveLevel ? "true" : "false") + ColorFormatter::popColor();
}
