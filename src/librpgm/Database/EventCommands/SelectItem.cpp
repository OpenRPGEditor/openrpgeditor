#include "Database/EventCommands/SelectItem.hpp"

#include "Database/Database.hpp"

SelectItemCommand::SelectItemCommand(const std::optional<int>& _indent, nlohmann::json& parameters)
: IEventCommand(_indent, parameters) {
  parameters[0].get_to(item);
  parameters[1].get_to(type);
}

std::string SelectItemCommand::stringRep(const Database& db) const {
  std::string itm = db.system.variable(item);
  itm = itm.empty() ? std::format("#{:04}", item) : itm;
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Select Item" + colon.data() +
         itm + ", " + DecodeEnumName(type) + ColorFormatter::popColor();
}
