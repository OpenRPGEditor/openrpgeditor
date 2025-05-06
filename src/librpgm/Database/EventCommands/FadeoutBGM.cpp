#include "Database/EventCommands/FadeoutBGM.hpp"

#include "Database/Database.hpp"

FadeoutBGM::FadeoutBGM(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(duration);
}

void FadeoutBGM::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(duration); }

std::string FadeoutBGM::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Fadeout BGM") + colon.data() + db.secondsText(duration) + ColorFormatter::popColor(colored);
}