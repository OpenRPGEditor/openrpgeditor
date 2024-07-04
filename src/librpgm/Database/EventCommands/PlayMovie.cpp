#include "Database/EventCommands/PlayMovie.hpp"

std::string PlayMovieCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play Movie" + colon.data() +
         (name == "" ? "None" : name) + ColorFormatter::popColor();
}
