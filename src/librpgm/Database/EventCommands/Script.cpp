#include "Database/EventCommands/Script.hpp"

ScriptCommand::ScriptCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(script);
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
