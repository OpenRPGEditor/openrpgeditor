#include "Database/EventCommands/ChangeTileset.hpp"

#include "Database/Database.hpp"

std::string ChangeTilesetCommand::stringRep(const Database& db) const {
  const auto tilesetName = db.tilesetNameOrId(tileset);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Tileset" + colon.data() +
         tilesetName + ColorFormatter::popColor();
}
