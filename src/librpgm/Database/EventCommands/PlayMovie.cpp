#include "Database/EventCommands/PlayMovie.hpp"

#include "Database/Database.hpp"

PlayMovieCommand::PlayMovieCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(name);
}

void PlayMovieCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(name); }

std::string PlayMovieCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Play Movie") + colon.data() + db.imageText(name) + ColorFormatter::popColor(colored);
}
