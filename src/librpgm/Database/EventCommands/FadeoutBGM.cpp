#include "Database/EventCommands/FadeoutBGM.hpp"

FadeoutBGM::FadeoutBGM(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(duration);
}

std::string FadeoutBGM::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Fadeout BGM" + colon.data() +
         std::to_string(duration) + " seconds" + ColorFormatter::popColor();
}