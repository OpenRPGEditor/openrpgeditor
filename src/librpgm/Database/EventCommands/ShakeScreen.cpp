#include "Database/EventCommands/ShakeScreen.hpp"

ShakeScreenCommand::ShakeScreenCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(power);
  parameters[1].get_to(speed);
  parameters[2].get_to(duration);
  parameters[3].get_to(waitForCompletion);
}

std::string ShakeScreenCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Shake Screen" + colon.data() +
         std::format("{}, {}, {} frames", power, speed, duration) + ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
