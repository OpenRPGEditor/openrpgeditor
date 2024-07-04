#include "Database/EventCommands/PlaySE.hpp"

std::string PlaySECommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play SE" + colon.data() +
         (audio.name == "" ? "None" : audio.name) + " " +
         std::format("({}, {}, {})", audio.volume, audio.pitch, audio.pan) + ColorFormatter::popColor();
}
