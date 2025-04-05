#include "Database/EventCommands/PlayMovie.hpp"

PlayMovieCommand::PlayMovieCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(name);
}

void PlayMovieCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(name); }

std::string PlayMovieCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Play Movie" + colon.data() + (name == "" ? "None" : name) + ColorFormatter::popColor();
}
