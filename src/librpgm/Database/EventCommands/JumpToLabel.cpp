#include "Database/EventCommands/JumpToLabel.hpp"

JumpToLabelCommand::JumpToLabelCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) { parameters.at(0).get_to(label); }

void JumpToLabelCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(label); }

std::string JumpToLabelCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Jump to Label" + colon.data() + label + ColorFormatter::popColor();
}
