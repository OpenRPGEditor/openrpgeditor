#include "Database/EventCommands/ChangeWindowColor.hpp"

ChangeWindowColorCommand::ChangeWindowColorCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  auto colors = parameters[0];
  colors[0].get_to(r);
  colors[1].get_to(g);
  colors[2].get_to(b);
}

void ChangeWindowColorCommand::serializeParameters(nlohmann::json& out) const {
  auto colors = nlohmann::json();
  colors.push_back(r);
  colors.push_back(g);
  colors.push_back(b);
  out.push_back(colors);
}

std::string ChangeWindowColorCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Window Color" +
         colon.data() + std::format("({}, {}, {})", r, g, b) + ColorFormatter::popColor();
}