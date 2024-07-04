#include "ChangeName.hpp"

#include "Database/Database.hpp"

std::string ChangeNameCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Name" + colon.data() +
         db.actorNameOrId(actor) + ", " + name + ColorFormatter::popColor();
}
