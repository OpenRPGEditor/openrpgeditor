#include "Database/EventCommands/PlaySE.hpp"

PlaySECommand::PlaySECommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(audio);
}

std::string PlaySECommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play SE" + colon.data() +
         (audio.name == "" ? "None" : audio.name) + " " +
         std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan) + ColorFormatter::popColor();
}
