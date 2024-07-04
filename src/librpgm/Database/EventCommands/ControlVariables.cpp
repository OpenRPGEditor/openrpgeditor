#include "Database/EventCommands/ControlVariables.hpp"
#include "Database/Database.hpp"

std::string ControlVariables::variableFormat(const std::string& text) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + DecodeEnumName(code()) +
         colon.data() + text + ColorFormatter::popColor();
}

std::string ControlVariables::stringRep(const Database& db) const {
  std::string varText;
  if (start != end) {
    varText = std::format("#{:04}..#{:04} {}", start, end, DecodeEnumName(operation));
  } else {
    varText = std::format("#{:04} {} {}", start, db.system.variable(start), DecodeEnumName(operation));
  }

  switch (operand) {
  case VariableControlOperand::Constant:
    return variableFormat(std::format("{} {}", varText, constant));
  case VariableControlOperand::Random: {
    return variableFormat(std::format("{} Random {}..{}", varText, random.min, random.max));
  }
  case VariableControlOperand::Variable: {
    return variableFormat(std::format("{} #{:04} {}", varText, variable, db.system.variable(variable)));
  }
  case VariableControlOperand::Game_Data: {
    switch (gameData.source) {
    case GameDataSource::Item: {
      return variableFormat(std::format("{} The number of {}", varText, db.itemNameOrId(gameData.value)));
    }
    case GameDataSource::Weapon: {
      return variableFormat(std::format("{} The number of {}", varText, db.weaponNameOrId(gameData.value)));
    }
    case GameDataSource::Armor: {
      return variableFormat(std::format("{} The number of {}", varText, db.armorNameOrId(gameData.value)));
    }
    case GameDataSource::Actor: {
      return variableFormat(
          std::format("{} {} of {}", varText, DecodeEnumName(gameData.actorSource), db.actorNameOrId(gameData.value)));
    }
    case GameDataSource::Enemy: {
      return variableFormat(std::format("{} {} of #{}", varText, DecodeEnumName(gameData.enemySource), gameData.value + 1));
    }
    case GameDataSource::Character: {
      const auto map = db.mapInfos.currentMap();
      const auto ev = map->event(gameData.value);
      std::string name = gameData.value == -1      ? "Player"
                         : gameData.value == 0     ? "This Event"
                         : ev && !ev->name.empty() ? ev->name
                                                   : std::format("#{:03}", gameData.value);
      return variableFormat(std::format("{} {} of {}", varText, DecodeEnumName(gameData.characterSource), name));
    }
    case GameDataSource::Party: {
      return variableFormat(std::format("{} Actor ID of the party member #{}", varText, gameData.value + 1));
    }
    case GameDataSource::Other: {
      return variableFormat(std::format("{} {}", varText, DecodeEnumName(gameData.otherSource)));
    }
    }
  }
  case VariableControlOperand::Script:
    return variableFormat(std::format("{} {}", varText, script));
  }

  return "ERROR!";
}