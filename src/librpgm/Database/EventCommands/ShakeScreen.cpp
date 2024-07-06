#include "Database/EventCommands/ShakeScreen.hpp"

ShakeScreenCommand::ShakeScreenCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(power);
  parameters.at(1).get_to(speed);
  parameters.at(2).get_to(duration);
  parameters.at(3).get_to(waitForCompletion);
}

void ShakeScreenCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(power);
  out.push_back(speed);
  out.push_back(duration);
  out.push_back(waitForCompletion);
}

std::string ShakeScreenCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Shake Screen" + colon.data() +
         std::format("{}, {}, {} frames", power, speed, duration) + ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
