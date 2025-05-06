#include "Database/EventCommands/ScrollingText.hpp"

NextScrollingTextCommand::NextScrollingTextCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(text);
}

void NextScrollingTextCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(text); }

ShowScrollTextCommand::ShowScrollTextCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(speed);
  parameters.at(1).get_to(noFast);
}

void ShowScrollTextCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(speed);
  out.push_back(noFast);
}

std::string ShowScrollTextCommand::stringRep(const Database& db, const bool colored) const {
  const std::string name = trNOOP("Text(S)");
  std::string ret = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + name.data() + colon.data() + ColorFormatter::popColor(colored) +
                    ColorFormatter::getColor(FormatColor::Gray, colored) + trNOOP("Speed") + " " + std::to_string(speed) + (noFast == true ? ", " + tr("No Fast Forward") : "") +
                    ColorFormatter::popColor(colored);

  for (const auto& t : text) {
    if (!ret.empty()) {
      ret += "\n";
    }
    ret += indentText(indent()) + colon.data() + ColorFormatter::getColorCode(t->code(), colored) + std::string(name.length(), ' ') + colon.data() + t->text + ColorFormatter::popColor(colored);
  }
  return ret;
}
