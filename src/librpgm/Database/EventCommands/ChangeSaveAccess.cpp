#include "Database/EventCommands/ChangeSaveAccess.hpp"

ChangeSaveAccessCommand::ChangeSaveAccessCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(access);
}

void ChangeSaveAccessCommand::serializeParameters(nlohmann::json& out) const { out.push_back(access); }

std::string ChangeSaveAccessCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Save Access" +
         colon.data() + DecodeEnumName(access) + ColorFormatter::popColor();
}
