#include "Database/EventCommands/ScrollingText.hpp"

NextScrollingTextCommand::NextScrollingTextCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(text);
}

ShowScrollTextCommand::ShowScrollTextCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(speed);
  parameters[1].get_to(noFast);
}

std::string ShowScrollTextCommand::stringRep(const Database& db) const {
  static constexpr std::string_view name = "Text(S)";
  std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + name.data() +
                    colon.data() + ColorFormatter::popColor() + ColorFormatter::getColor(Color::Gray) + " Speed " +
                    std::to_string(speed) + (noFast == true ? ", No Fast Forward" : "") + ColorFormatter::popColor();

  for (const auto& t : text) {
    if (!ret.empty()) {
      ret += "\n";
    }
    ret += indentText(indent) + colon.data() + ColorFormatter::getColorCode(t->code()) +
           std::string(name.length(), ' ') + colon.data() + t->text + ColorFormatter::popColor();
  }
  return ret;
}
