#include "Database/EventCommands/PlayBGM.hpp"

PlayBGMCommand::PlayBGMCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(audio);
}

std::string PlayBGMCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play BGM" + colon.data() +
         (audio.name == "" ? "None" : audio.name) + " " +
         std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan) + ColorFormatter::popColor();
}
