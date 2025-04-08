#include "Database/EventCommands/SelectItem.hpp"

#include "Database/Database.hpp"

SelectItemCommand::SelectItemCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters)
: IEventCommand(_indent, parameters) {
  parameters.at(0).get_to(item);
  parameters.at(1).get_to(type);
}

void SelectItemCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(item);
  out.push_back(type);
}

std::string SelectItemCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Select Item") + colon.data() + db.variableNameOrId(item) + ", " + DecodeEnumName(type) +
         ColorFormatter::popColor(colored);
}
