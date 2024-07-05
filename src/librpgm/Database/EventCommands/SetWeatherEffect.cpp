#include "Database/EventCommands/SetWeatherEffect.hpp"

SetWeatherEffectCommand::SetWeatherEffectCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  std::string eff;
  parameters[0].get_to(eff);
  effect = magic_enum::enum_cast<WeatherEffect>(eff, magic_enum::case_insensitive).value();
  parameters[1].get_to(power);
  parameters[2].get_to(duration);
  parameters[3].get_to(waitForCompletion);
}

std::string SetWeatherEffectCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Set Weather Effect" +
         colon.data() + DecodeEnumName(effect) + ", " + std::to_string(power) + ", " + std::to_string(duration) +
         " frames" + ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
