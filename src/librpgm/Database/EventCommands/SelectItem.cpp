#include "Database/EventCommands/SelectItem.hpp"

#include "Database/Database.hpp"

std::string SelectItemCommand::stringRep(const Database& db) const {
  std::string itm = db.system.variable(item);
  itm = itm.empty() ? std::format("#{:04}", item) : itm;
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Select Item" + colon.data() +
         itm + ", " + DecodeEnumName(type) + ColorFormatter::popColor();
}
