#include "Database/EventCommands/ChangeMenuAccess.hpp"

ChangeMenuAccessCommand::ChangeMenuAccessCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(access);
}

void ChangeMenuAccessCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(access); }

std::string ChangeMenuAccessCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Menu Access") + colon.data() + DecodeEnumName(access) +
         ColorFormatter::popColor(colored);
}
