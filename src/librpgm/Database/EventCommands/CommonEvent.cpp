#include "Database/EventCommands/CommonEvent.hpp"

#include "Database/Database.hpp"

CommonEventCommand::CommonEventCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(event);
}

void CommonEventCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(event); }

std::string CommonEventCommand::stringRep(const Database& db, const bool colored) const {
  const auto eventName = db.commonEventNameOrId(event);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Common Event") + colon.data() + eventName + ColorFormatter::popColor(colored);
}
