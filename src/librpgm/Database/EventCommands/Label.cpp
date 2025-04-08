#include "Database/EventCommands/Label.hpp"

LabelCommand::LabelCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(label);
}

void LabelCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(label); }

std::string LabelCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Label") + colon.data() + label + ColorFormatter::popColor(colored);
}
