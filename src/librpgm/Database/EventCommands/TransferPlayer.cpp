#include "Database/EventCommands/TransferPlayer.hpp"

#include "Database/Database.hpp"

TransferPlayerCommand::TransferPlayerCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(mode);
  parameters.at(1).get_to(mapId);
  parameters.at(2).get_to(x);
  parameters.at(3).get_to(y);
  parameters.at(4).get_to(direction);
  parameters.at(5).get_to(fade);
}

void TransferPlayerCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(mode);
  out.push_back(mapId);
  out.push_back(x);
  out.push_back(y);
  out.push_back(direction);
  out.push_back(fade);
}

std::string TransferPlayerCommand::stringRep(const Database& db, const bool colored) const {
  std::string suffix;

  if (direction != Direction::Retain || fade != Fade::Black) {
    suffix += ColorFormatter::getColor(FormatColor::Gray, colored) + " (";
  }
  if (direction != Direction::Retain) {
    suffix += tr("Direction") + ": " + DecodeEnumName(direction);
  }

  if (fade != Fade::Black) {
    if (direction != Direction::Retain) {
      suffix += ", ";
    }
    suffix += tr("Fade") + ": " + DecodeEnumName(fade);
  }
  if (direction != Direction::Retain || fade != Fade::Black) {
    suffix += ")" + ColorFormatter::popColor(colored);
  }

  if (mode == TransferMode::Variable_Designation) {
    auto varMap = db.variableNameOrId(mapId);
    auto varX = db.variableNameOrId(x);
    auto varY = db.variableNameOrId(y);
    return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Transfer Player") + colon.data() +
           std::format("{{{}}} ({{{}}},{{{}}})", varMap, varX, varY) + ColorFormatter::popColor(colored) + suffix;
  }
  const auto mapName = db.mapNameOrId(mapId);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Transfer Player") + colon.data() + mapName + std::format(" ({},{})", x, y) +
         ColorFormatter::popColor(colored) + suffix;
}
