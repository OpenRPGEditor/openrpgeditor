#include "Database/EventCommands/ChangeTileset.hpp"
#include "Database/Database.hpp"

ChangeTilesetCommand::ChangeTilesetCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) { parameters.at(0).get_to(tileset); }

void ChangeTilesetCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(tileset); }

std::string ChangeTilesetCommand::stringRep(const Database& db) const {
  const auto tilesetName = db.tilesetNameOrId(tileset);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Tileset" + colon.data() + tilesetName + ColorFormatter::popColor();
}
