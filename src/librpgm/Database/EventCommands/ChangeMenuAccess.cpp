#include "Database/EventCommands/ChangeMenuAccess.hpp"

ChangeMenuAccessCommand::ChangeMenuAccessCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(access);
}

std::string ChangeMenuAccessCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Menu Access" +
         colon.data() + DecodeEnumName(access) + ColorFormatter::popColor();
}
