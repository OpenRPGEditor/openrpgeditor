#include "Database/EventCommands/ForceAction.hpp"

#include "Database/Database.hpp"

ForceActionCommand::ForceActionCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
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

std::string ForceActionCommand::stringRep(const Database& db) const {
  const std::string targetString = target == -2 ? "Last Target" : target == -1 ? "Random" : "Index " + std::to_string(target + 1);
  const auto attack = db.skills.skill(skill);
  if (sourceComparison == SubjectComparisonSource::Enemy) {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Force Action" + colon.data() + "#" + std::to_string(source + 1) + ", " + attack->name + ", " + targetString +
           ColorFormatter::popColor();
  }
  const auto actor = db.actors.actor(source);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Force Action" + colon.data() + actor->name + ", " + attack->name + ", " + targetString +
         ColorFormatter::popColor();
}
