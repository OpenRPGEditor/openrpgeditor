#include "Database/EventCommands/ScrollingText.hpp"

NextScrollingTextCommand::NextScrollingTextCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(text);
}

void NextScrollingTextCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(text); }

ShowScrollTextCommand::ShowScrollTextCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(speed);
  parameters.at(1).get_to(noFast);
}

void ShowScrollTextCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(speed);
  out.push_back(noFast);
}

std::string ShowScrollTextCommand::stringRep(const Database& db) const {
  static constexpr std::string_view name = "Text(S)";
  std::string ret = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + name.data() + colon.data() + ColorFormatter::popColor() +
                    ColorFormatter::getColor(FormatColor::Gray) + "Speed " + std::to_string(speed) + (noFast == true ? ", No Fast Forward" : "") + ColorFormatter::popColor();

  for (const auto& t : text) {
    if (!ret.empty()) {
      ret += "\n";
    }
    ret += indentText(indent()) + colon.data() + ColorFormatter::getColorCode(t->code()) + std::string(name.length(), ' ') + colon.data() + t->text + ColorFormatter::popColor();
  }
  return ret;
}
