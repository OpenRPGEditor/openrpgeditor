#include "Database/EventCommands/SetWeatherEffect.hpp"

SetWeatherEffectCommand::SetWeatherEffectCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
  std::string eff;
  parameters.at(0).get_to(eff);
  effect = magic_enum::enum_cast<WeatherEffect>(eff, magic_enum::case_insensitive).value();
  parameters.at(1).get_to(power);
  parameters.at(2).get_to(duration);
  parameters.at(3).get_to(waitForCompletion);
}

void SetWeatherEffectCommand::serializeParameters(nlohmann::ordered_json& out) const {
  std::string eff = magic_enum::enum_name(effect).data();
  std::ranges::transform(eff, eff.begin(), [](const char c) { return std::tolower(c); });
  out.push_back(eff);
  out.push_back(power);
  out.push_back(duration);
  out.push_back(waitForCompletion);
}

std::string SetWeatherEffectCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Set Weather Effect" + colon.data() + DecodeEnumName(effect) + ", " + std::to_string(power) + ", " +
         std::to_string(duration) + " frames" + ColorFormatter::popColor() + (waitForCompletion == true ? ColorFormatter::getColor(FormatColor::Gray) + " (Wait)" + ColorFormatter::popColor() : "");
}
