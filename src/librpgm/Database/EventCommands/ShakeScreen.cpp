#include "Database/EventCommands/ShakeScreen.hpp"

std::string ShakeScreenCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Shake Screen" + colon.data() +
         std::format("{}, {}, {} frames", power, speed, duration) + ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
