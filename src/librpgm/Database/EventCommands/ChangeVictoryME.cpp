#include "Database/EventCommands/ChangeVictoryME.hpp"

ChangeVictoryMECommand::ChangeVictoryMECommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(me);
}

void ChangeVictoryMECommand::serializeParameters(nlohmann::json& out) const { out.push_back(me); }

std::string ChangeVictoryMECommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Victory ME" +
         colon.data() + (me.name == "" ? "None" : me.name) + " " +
         std::format("({}, {}, {})", me.volume, me.pitch, me.pan) + ColorFormatter::popColor();
}
