#include "Database/EventCommands/FadeoutBGM.hpp"

FadeoutBGM::FadeoutBGM(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(duration);
}

void FadeoutBGM::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(duration); }

std::string FadeoutBGM::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Fadeout BGM" + colon.data() +
         std::to_string(duration) + " seconds" + ColorFormatter::popColor();
}