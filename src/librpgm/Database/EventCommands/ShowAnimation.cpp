#include "Database/EventCommands/ShowAnimation.hpp"

#include "Database/Database.hpp"

ShowAnimationCommand::ShowAnimationCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(character);
  parameters[1].get_to(animation);
  parameters[2].get_to(waitForCompletion);
}

void ShowAnimationCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(character);
  out.push_back(animation);
  out.push_back(waitForCompletion);
}

std::string ShowAnimationCommand::stringRep(const Database& db) const {
  const auto evName = db.eventNameOrId(character);
  const auto animName = db.animationNameOrId(animation);

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Animation" + colon.data() +
         evName + ", " + animName + " " + ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + "(Wait)" + ColorFormatter::popColor()
                                    : "");
}