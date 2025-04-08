#include "Database/EventCommands/ForceAction.hpp"

#include "Database/Database.hpp"

ForceActionCommand::ForceActionCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(sourceComparison);
  parameters.at(1).get_to(source);
  parameters.at(2).get_to(skill);
  parameters.at(3).get_to(target);
}

void ForceActionCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(sourceComparison);
  out.push_back(source);
  out.push_back(skill);
  out.push_back(target);
}

std::string ForceActionCommand::stringRep(const Database& db, const bool colored) const {
  const std::string targetString = target == -2 ? trNOOP("Last Target") : target == -1 ? trNOOP("Random") : tr("Index") + " " + std::to_string(target + 1);
  const auto attack = db.skillNameOrId(skill);
  if (sourceComparison == SubjectComparisonSource::Enemy) {
    return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Force Action") + colon.data() + "#" + std::to_string(source + 1) + ", " + attack + ", " +
           targetString + ColorFormatter::popColor(colored);
  }
  const auto actor = db.actorNameOrId(source);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Force Action") + colon.data() + actor + ", " + attack + ", " + targetString +
         ColorFormatter::popColor(colored);
}
