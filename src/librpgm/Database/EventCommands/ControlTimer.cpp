#include "Database/EventCommands/ControlTimer.hpp"

ControlTimerCommand::ControlTimerCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(control);
  if (control == TimerControl::Start) {
    parameters.at(1).get_to(seconds);
  }
}

void ControlTimerCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(control);
  if (control == TimerControl::Start) {
    out.push_back(seconds);
  }
}

std::string ControlTimerCommand::stringRep(const Database& db, const bool colored) const {
  std::string min;
  std::string sec;

  if (seconds > 59) {
    min = std::to_string(seconds / 60);
    sec = std::to_string(seconds % 60);
  } else {
    min = "0";
    sec = std::to_string(seconds);
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Control Timer") + colon.data() + DecodeEnumName(control) + ", " +
         // TL-NOTE: Minutes and Seconds
         trFormat("{} min {} sec", min, sec) + ColorFormatter::popColor(colored);
}
