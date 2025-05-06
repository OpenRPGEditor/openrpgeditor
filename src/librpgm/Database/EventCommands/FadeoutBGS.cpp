#include "Database/EventCommands/FadeoutBGS.hpp"

#include "Database/Database.hpp"

FadeoutBGS::FadeoutBGS(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(duration);
}

void FadeoutBGS::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(duration); }

std::string FadeoutBGS::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Fadeout BGS") + colon.data() + db.secondsText(duration) + ColorFormatter::popColor(colored);
}
