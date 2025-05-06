#include "Database/EventCommands/ControlVariables.hpp"
#include "Database/Database.hpp"

ControlVariables::ControlVariables(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(start);
  parameters.at(1).get_to(end);
  parameters.at(2).get_to(operation);
  parameters.at(3).get_to(operand);
  switch (operand) {
  case VariableControlOperand::Constant:
    parameters.at(4).get_to(constant);
    break;
  case VariableControlOperand::Variable:
    parameters.at(4).get_to(variable);
    break;
  case VariableControlOperand::Random:
    parameters.at(4).get_to(random.min);
    parameters.at(5).get_to(random.max);
    break;
  case VariableControlOperand::Game_Data:
    parameters.at(4).get_to(gameData.type);
    parameters.at(5).get_to(gameData.value);
    parameters.at(6).get_to(gameData.rawSource);
    break;
  case VariableControlOperand::Script:
    parameters.at(4).get_to(script);
    break;
  }
}

void ControlVariables::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(start);
  out.push_back(end);
  out.push_back(operation);
  out.push_back(operand);
  switch (operand) {
  case VariableControlOperand::Constant:
    out.push_back(constant);
    break;
  case VariableControlOperand::Variable:
    out.push_back(variable);
    break;
  case VariableControlOperand::Random:
    out.push_back(random.min);
    out.push_back(random.max);
    break;
  case VariableControlOperand::Game_Data:
    out.push_back(gameData.type);
    out.push_back(gameData.value);
    out.push_back(gameData.rawSource);
    break;
  case VariableControlOperand::Script:
    out.push_back(script);
    break;
  }
}

std::string ControlVariables::variableFormat(const std::string& text, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + DecodeEnumName(code()) + colon.data() + text + ColorFormatter::popColor(colored);
}

std::string ControlVariables::stringRep(const Database& db, const bool colored) const {
  std::string varText;
  if (start != end) {
    varText = std::format("#{:04}..#{:04} {}", start, end, DecodeEnumName(operation));
  } else {
    varText = std::format("{} {}", db.variableNameAndId(start), DecodeEnumName(operation));
  }

  switch (operand) {
  case VariableControlOperand::Constant:
    return variableFormat(std::format("{} {}", varText, constant), colored);
  case VariableControlOperand::Random: {
    return variableFormat(varText + " " + trNOOP("Random") + std::format(" {}..{}", random.min, random.max), colored);
  }
  case VariableControlOperand::Variable: {
    return variableFormat(varText + " " + db.variableNameOrId(variable), colored);
  }
  case VariableControlOperand::Game_Data: {
    switch (gameData.type) {
    case GameDataType::Item: {
      return variableFormat(varText + " " + Database::theNumberOf(db.itemNameOrId(gameData.value)), colored);
    }
    case GameDataType::Weapon: {
      return variableFormat(varText + " " + Database::theNumberOf(db.weaponNameOrId(gameData.value)), colored);
    }
    case GameDataType::Armor: {
      return variableFormat(varText + " " + Database::theNumberOf(db.armorNameOrId(gameData.value)), colored);
    }
    case GameDataType::Actor: {
      return variableFormat(varText + " " + Database::AOfB(DecodeEnumName(gameData.actorSource), db.actorNameOrId(gameData.value)), colored);
    }
    case GameDataType::Enemy: {
      std::string enemyName;
      if (m_troopId == 0) {
        enemyName = std::format("#{}", gameData.value + 1);
      } else {
        enemyName = db.troopMemberName(m_troopId, gameData.value);
      }
      return variableFormat(varText + " " + Database::AOfB(DecodeEnumName(gameData.enemySource), enemyName), colored);
    }
    case GameDataType::Character: {
      return variableFormat(varText + " " + Database::AOfB(DecodeEnumName(gameData.characterSource), db.eventNameOrId(gameData.value)), colored);
    }
    case GameDataType::Party: {
      return variableFormat(varText + " " + trFormat("Actor ID of the party member #{0}", gameData.value + 1), colored);
    }
    case GameDataType::Other: {
      return variableFormat(varText + " " + DecodeEnumName(gameData.otherSource), colored);
    }
    }
  }
  case VariableControlOperand::Script:
    std::string tmpScript = script;
    std::ranges::replace(tmpScript, '\n', ' ');
    return variableFormat(varText + " " + tmpScript.substr(0, std::min<size_t>(80, tmpScript.length())) + (tmpScript.length() >= 80 ? "..." : ""), colored);
  }

  return "ERROR!";
}