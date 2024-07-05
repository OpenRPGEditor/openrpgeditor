#include "Database/EventCommands/PlayMovie.hpp"

PlayMovieCommand::PlayMovieCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(name);
}

std::string PlayMovieCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play Movie" + colon.data() +
         (name == "" ? "None" : name) + ColorFormatter::popColor();
}
