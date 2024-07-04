#include "Database/EventCommands/SetWeatherEffect.hpp"

std::string SetWeatherEffectCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Set Weather Effect" +
         colon.data() + DecodeEnumName(effect) + ", " + std::to_string(power) + ", " + std::to_string(duration) +
         " frames" + ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
