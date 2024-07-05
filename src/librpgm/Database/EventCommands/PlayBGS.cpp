#include "Database/EventCommands/PlayBGS.hpp"

PlayBGSCommand::PlayBGSCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(audio);
}

std::string PlayBGSCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play BGS" + colon.data() +
         (audio.name == "" ? "None" : audio.name) + " " +
         std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan) + ColorFormatter::popColor();
}
