#include "Database/EventCommands/ChangeSkill.hpp"
#include "Database/Database.hpp"

ChangeSkillCommand::ChangeSkillCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(comparison);
  parameters.at(1).get_to(value);
  parameters.at(2).get_to(skillOp);
  parameters.at(3).get_to(skill);
}

void ChangeSkillCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(comparison);
  out.push_back(value);
  out.push_back(skillOp);
  out.push_back(skill);
}

std::string ChangeSkillCommand::stringRep(const Database& db, const bool colored) const {
  std::string actorName;
  if (comparison == ActorComparisonSource::Variable) {
    actorName = db.system.variable(value);
    actorName = actorName.empty() ? std::format("{{{:04}}}", value) : actorName;
  } else {
    actorName = value == 0 ? trNOOP("Entire Party") : db.actorNameOrId(value);
  }

  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Skill") + colon.data() + actorName + ", " + DecodeEnumName(skillOp) + " " +
         db.skillNameOrId(skill) + ColorFormatter::popColor(colored);
}