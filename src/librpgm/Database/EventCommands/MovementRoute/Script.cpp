#include "Database/EventCommands/MovementRoute/Script.hpp"

std::string MovementScriptCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + DecodeEnumName(code()) + colon.data() + script;
}
