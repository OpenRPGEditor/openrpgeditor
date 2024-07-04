#include "ShowChoice.hpp"

std::string ShowChoiceCommand::stringRep(const Database& db) const {
  std::string suffix =
      ColorFormatter::getColor(Color::Gray) + " (" + DecodeEnumName(background) + ", " + DecodeEnumName(positionType);
  suffix += (defaultType < 0 ? ", -" : ", #" + std::to_string(defaultType + 1));
  suffix += (cancelType < 0 ? ", -" : ", #" + std::to_string(cancelType + 1)) + ")" + ColorFormatter::popColor();

  std::string choiceList = std::accumulate(std::next(choices.begin()), choices.end(), *choices.begin(),
                                           [](const std::string& a, const std::string& b) { return a + ", " + b; });

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Choices" + colon.data() +
         choiceList + ColorFormatter::popColor() + suffix;
}

std::string WhenSelectedCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "When " + choice +
         ColorFormatter::popColor();
}

std::string WhenCancelCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "When Cancel" +
         ColorFormatter::popColor();
}