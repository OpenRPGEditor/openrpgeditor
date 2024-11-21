#include "Database/EventCommands/FadeoutBGS.hpp"

FadeoutBGS::FadeoutBGS(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(duration);
}

void FadeoutBGS::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(duration); }

std::string FadeoutBGS::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Fadeout BGS" + colon.data() +
         std::to_string(duration) + " seconds" + ColorFormatter::popColor();
}
