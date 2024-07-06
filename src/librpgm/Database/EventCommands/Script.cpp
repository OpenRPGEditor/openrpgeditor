#include "Database/EventCommands/Script.hpp"

NextScriptCommand::NextScriptCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(script);
}

void NextScriptCommand::serializeParameters(nlohmann::json& out) const { out.push_back(script); }

ScriptCommand::ScriptCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(script);
}

void ScriptCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(script);
}

std::string ScriptCommand::stringRep(const Database& db) const {
  const auto name = UndectorateEnumName(code());
  std::string ret = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + name + colon.data() +
                    script + ColorFormatter::popColor();
  for (const auto& t : moreScript) {
    if (!ret.empty()) {
      ret += "\n";
    }
    ret += indentText(indent) + symbol(t->code()) + ColorFormatter::getColorCode(t->code()) +
           std::string(name.length(), ' ') + colon.data() + t->script + ColorFormatter::popColor();
  }

  return ret;
}
