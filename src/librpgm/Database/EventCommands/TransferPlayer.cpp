#include "Database/EventCommands/TransferPlayer.hpp"

#include "Database/Database.hpp"

TransferPlayerCommand::TransferPlayerCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(mode);
  parameters[1].get_to(mapId);
  parameters[2].get_to(x);
  parameters[3].get_to(y);
  parameters[4].get_to(direction);
  parameters[5].get_to(fade);
}

std::string TransferPlayerCommand::stringRep(const Database& db) const {
  std::string suffix;

  if (direction != Direction::Retain || fade != Fade::Black) {
    suffix += ColorFormatter::getColor(Color::Gray) + "(";
  }
  if (direction != Direction::Retain) {
    suffix += "Direction: " + DecodeEnumName(direction);
  }

  if (fade != Fade::Black) {
    if (direction != Direction::Retain) {
      suffix += ", ";
    }
    suffix += "Fade: " + DecodeEnumName(fade);
  }
  if (direction != Direction::Retain || fade != Fade::Black) {
    suffix += ")" + ColorFormatter::popColor();
  }

  if (mode == TransferMode::Variable_Designation) {
    auto varMap = db.system.variable(mapId);
    auto varX = db.system.variable(x);
    auto varY = db.system.variable(y);
    varMap = varMap.empty() ? std::format("#{:04}", mapId) : varMap;
    varX = varX.empty() ? std::format("#{:04}", x) : varX;
    varY = varY.empty() ? std::format("#{:04}", y) : varY;
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Transfer Player" +
           colon.data() + std::format("{{{}}} ({{{}}},{{{}}})", varMap, varX, varY) + suffix;
  }
  const auto map = db.mapInfos.map(mapId);
  const auto mapName = map && !map->name.empty() ? map->name : std::format("#{:03}", mapId);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Transfer Player" + colon.data() +
         mapName + std::format(" ({},{})", x, y) + ColorFormatter::popColor() + suffix;
}
