#include "Database/EventCommands/FadeoutBGS.hpp"

FadeoutBGS::FadeoutBGS(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(duration);
}

std::string FadeoutBGS::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Fadeout BGS" + colon.data() +
         std::to_string(duration) + " seconds" + ColorFormatter::popColor();
}
