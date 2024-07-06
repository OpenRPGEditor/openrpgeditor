#include "Database/EventCommands/ControlVariables.hpp"
#include "Database/Database.hpp"

ControlVariables::ControlVariables(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(start);
  parameters[1].get_to(end);
  parameters[2].get_to(operation);
  parameters[3].get_to(operand);
  switch (operand) {
  case VariableControlOperand::Constant:
    parameters[4].get_to(constant);
    break;
  case VariableControlOperand::Variable:
    parameters[4].get_to(variable);
    break;
  case VariableControlOperand::Random:
    parameters[4].get_to(random.min);
    parameters[5].get_to(random.max);
    break;
  case VariableControlOperand::Game_Data:
    parameters[4].get_to(gameData.type);
    parameters[5].get_to(gameData.value);
    parameters[6].get_to(gameData.rawSource);
    break;
  case VariableControlOperand::Script:
    parameters[4].get_to(script);
    break;
  }
}

void ControlVariables::serializeParameters(nlohmann::json& out) const {
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

std::string ControlVariables::variableFormat(const std::string& text) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + DecodeEnumName(code()) +
         colon.data() + text + ColorFormatter::popColor();
}

std::string ControlVariables::stringRep(const Database& db) const {
  static constexpr std::string_view numberOf("{} The number of {}");
  static constexpr std::string_view AOfB("{} {} of {}");
  std::string varText;
  if (start != end) {
    varText = std::format("#{:04}..#{:04} {}", start, end, DecodeEnumName(operation));
  } else {
    varText = std::format("#{} {}", db.variableNameAndId(start), DecodeEnumName(operation));
  }

  switch (operand) {
  case VariableControlOperand::Constant:
    return variableFormat(std::format("{} {}", varText, constant));
  case VariableControlOperand::Random: {
    return variableFormat(std::format("{} Random {}..{}", varText, random.min, random.max));
  }
  case VariableControlOperand::Variable: {
    return variableFormat(std::format("{} ", varText, variable, db.system.variable(variable)));
  }
  case VariableControlOperand::Game_Data: {
    switch (gameData.type) {
    case GameDataType::Item: {
      return variableFormat(std::format(numberOf, varText, db.itemNameOrId(gameData.value)));
    }
    case GameDataType::Weapon: {
      return variableFormat(std::format(numberOf, varText, db.weaponNameOrId(gameData.value)));
    }
    case GameDataType::Armor: {
      return variableFormat(std::format(numberOf, varText, db.armorNameOrId(gameData.value)));
    }
    case GameDataType::Actor: {
      return variableFormat(
          std::format(AOfB, varText, DecodeEnumName(gameData.actorSource), db.actorNameOrId(gameData.value)));
    }
    case GameDataType::Enemy: {
      return variableFormat(std::format(AOfB, varText, DecodeEnumName(gameData.enemySource), gameData.value + 1));
    }
    case GameDataType::Character: {
      return variableFormat(
          std::format(AOfB, varText, DecodeEnumName(gameData.characterSource), db.eventNameOrId(gameData.value)));
    }
    case GameDataType::Party: {
      return variableFormat(std::format("{} Actor ID of the party member #{}", varText, gameData.value + 1));
    }
    case GameDataType::Other: {
      return variableFormat(std::format("{} {}", varText, DecodeEnumName(gameData.otherSource)));
    }
    }
  }
  case VariableControlOperand::Script:
    return variableFormat(std::format("{} {}", varText, script));
  }

  return "ERROR!";
}