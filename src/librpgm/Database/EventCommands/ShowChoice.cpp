#include "ShowChoice.hpp"

ShowChoiceCommand::ShowChoiceCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters)
: IEventCommand(_indent) {
  parameters.at(0).get_to(choices);
  parameters.at(1).get_to(cancelType);
  if (parameters.size() > 2) {
    parameters.at(2).get_to(defaultType);
  } else {
    defaultType = 0;
  }
  if (parameters.size() > 3) {
    parameters.at(3).get_to(positionType);
  } else {
    positionType = ChoiceWindowPosition::Right;
  }

  if (parameters.size() > 4) {
    parameters.at(4).get_to(background);
  } else {
    background = TextBackground::Window;
  }

  if (cancelType >= choices.size()) {
    cancelType = -2;
  }
}

void ShowChoiceCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(choices);
  out.push_back(cancelType);
  if (defaultType != 0 || positionType != ChoiceWindowPosition::Right || background != TextBackground::Window) {
    out.push_back(defaultType);
  }
  if (positionType != ChoiceWindowPosition::Right || background != TextBackground::Window) {
    out.push_back(positionType);
  }

  if (background != TextBackground::Window) {
    out.push_back(background);
  }
}

std::string ShowChoiceCommand::stringRep(const Database& db, const bool colored) const {
  std::string suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + " (" + DecodeEnumName(background) + ", " + DecodeEnumName(positionType);
  suffix += (defaultType < 0 ? ", -" : ", #" + std::to_string(defaultType + 1));
  suffix += (cancelType < 0 ? ", -" : ", #" + std::to_string(cancelType + 1)) + ")" + ColorFormatter::popColor(colored);

  std::string choiceList = std::accumulate(std::next(choices.begin()), choices.end(), *choices.begin(), [](const std::string& a, const std::string& b) { return a + ", " + b; });

  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Show Choices") + colon.data() + choiceList + ColorFormatter::popColor(colored) + suffix;
}

WhenSelectedCommand::WhenSelectedCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters)
: IEventCommand(_indent) {
  parameters.at(0).get_to(param1);
  parameters.at(1).get_to(choice);
}

void WhenSelectedCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(param1);
  out.push_back(choice);
}

std::string WhenSelectedCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("When") + " " + choice + ColorFormatter::popColor(colored);
}