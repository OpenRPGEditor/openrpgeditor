#include "Database/EventCommands/ChangeClass.hpp"

#include "Database/Database.hpp"

ChangeClassCommand::ChangeClassCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(actor);
  parameters.at(1).get_to(classId);
  parameters.at(2).get_to(saveLevel);
}

void ChangeClassCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(actor);
  out.push_back(classId);
  out.push_back(saveLevel);
}

std::string ChangeClassCommand::stringRep(const Database& db, const bool colored) const {
  const auto actName = db.nameOrId(db.actorName(actor), actor);
  const auto clsName = db.nameOrId(db.className(classId), classId);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Class") + colon.data() + actName + ", " + clsName + ", " +
         (saveLevel ? "true" : "false") + ColorFormatter::popColor(colored);
}
