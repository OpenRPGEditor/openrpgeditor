#include "Database/EventCommands/ChangeSaveAccess.hpp"

ChangeSaveAccessCommand::ChangeSaveAccessCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(access);
}

void ChangeSaveAccessCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(access); }

std::string ChangeSaveAccessCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Save Access") + colon.data() + DecodeEnumName(access) +
         ColorFormatter::popColor(colored);
}
