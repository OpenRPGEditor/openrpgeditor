#include "Database/EventCommands/ConditionalBranch.hpp"

#include "Database/Database.hpp"

ConditionalBranchCommand::ConditionalBranchCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(type);
  switch (type) {
  case ConditionType::Switch: {
    parameters.at(1).get_to(globalSwitch.switchIdx);
    parameters.at(2).get_to(globalSwitch.checkIfOn);
    break;
  }
  case ConditionType::Variable: {
    parameters.at(1).get_to(variable.id);
    parameters.at(2).get_to(variable.source);
    if (variable.source == VariableComparisonSource::Constant) {
      parameters.at(3).get_to(variable.constant);
    } else {
      parameters.at(3).get_to(variable.otherId);
    }
    parameters.at(4).get_to(variable.comparison);
    break;
  }
  case ConditionType::Self_Switch: {
    parameters.at(1).get_to(selfSw);
    parameters.at(2).get_to(selfSwitch.checkIfOn);
    break;
  }
  case ConditionType::Timer: {
    parameters.at(1).get_to(timer.comparison);
    parameters.at(2).get_to(timer.sec);
    break;
  }
  case ConditionType::Actor: {
    parameters.at(1).get_to(actor.id);
    parameters.at(2).get_to(actor.type);
    switch (actor.type) {
    case ActorConditionType::Name:
    case ActorConditionType::In_The_Party:
      if (parameters.size() > 3 && parameters.at(3).is_string()) {
        parameters.at(3).get_to(name);
      }
      break;
    case ActorConditionType::Class:
    case ActorConditionType::Skill:
    case ActorConditionType::Weapon:
    case ActorConditionType::Armor:
    case ActorConditionType::State:
      parameters.at(3).get_to(actor.checkId);
      break;
    default:
      break;
    }
    break;
  }
  case ConditionType::Enemy: {
    parameters.at(1).get_to(enemy.id);
    parameters.at(2).get_to(enemy.type);
    if (enemy.type == EnemyConditionType::State) {
      parameters.at(3).get_to(enemy.stateId);
    }
    break;
  }
  case ConditionType::Character: {
    parameters.at(1).get_to(character.id);
    parameters.at(2).get_to(character.facing);
    break;
  }
  case ConditionType::Gold: {
    parameters.at(1).get_to(gold.value);
    parameters.at(2).get_to(gold.type);
    break;
  }
  case ConditionType::Item: {
    parameters.at(1).get_to(item.id);
    break;
  }
  case ConditionType::Weapon:
  case ConditionType::Armor: {
    parameters.at(1).get_to(equip.equipId);
    parameters.at(2).get_to(equip.includeEquipment);
    break;
  }
  case ConditionType::Button: {
    std::string buttonText;
    parameters.at(1).get_to(buttonText);
    button = magic_enum::enum_cast<Button>(buttonText, magic_enum::case_insensitive).value_or(Button::OK);
    break;
  }
  case ConditionType::Script: {
    parameters.at(1).get_to(script);
    break;
  }
  case ConditionType::Vehicle: {
    parameters.at(1).get_to(vehicle.id);
    break;
  }
  default:
    break;
  }
}

void ConditionalBranchCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(type);
  switch (type) {
  case ConditionType::Switch: {
    out.push_back(globalSwitch.switchIdx);
    out.push_back(globalSwitch.checkIfOn);
    break;
  }
  case ConditionType::Variable: {
    out.push_back(variable.id);
    out.push_back(variable.source);
    if (variable.source == VariableComparisonSource::Constant) {
      out.push_back(variable.constant);
    } else {
      out.push_back(variable.otherId);
    }
    out.push_back(variable.comparison);
    break;
  }
  case ConditionType::Self_Switch: {
    out.push_back(selfSw);
    out.push_back(selfSwitch.checkIfOn);
    break;
  }
  case ConditionType::Timer: {
    out.push_back(timer.comparison);
    out.push_back(timer.sec);
    break;
  }
  case ConditionType::Actor: {
    out.push_back(actor.id);
    out.push_back(actor.type);
    switch (actor.type) {
    case ActorConditionType::Name:
    case ActorConditionType::In_The_Party:
      if (!name.empty()) {
        out.push_back(name);
      }
      break;
    case ActorConditionType::Class:
    case ActorConditionType::Skill:
    case ActorConditionType::Weapon:
    case ActorConditionType::Armor:
    case ActorConditionType::State:
      out.push_back(actor.checkId);
      break;
    default:
      break;
    }
    break;
  }
  case ConditionType::Enemy: {
    out.push_back(enemy.id);
    out.push_back(enemy.type);
    if (enemy.type == EnemyConditionType::State) {
      out.push_back(enemy.stateId);
    }
    break;
  }
  case ConditionType::Character: {
    out.push_back(character.id);
    out.push_back(character.facing);
    break;
  }
  case ConditionType::Gold: {
    out.push_back(gold.value);
    out.push_back(gold.type);
    break;
  }
  case ConditionType::Item: {
    out.push_back(item.id);
    break;
  }
  case ConditionType::Weapon:
  case ConditionType::Armor: {
    out.push_back(equip.equipId);
    out.push_back(equip.includeEquipment);
    break;
  }
  case ConditionType::Button: {
    std::string buttonText = magic_enum::enum_name(button).data();
    std::ranges::transform(buttonText, buttonText.begin(), [](const char c) { return std::tolower(c); });
    out.push_back(buttonText);
    break;
  }
  case ConditionType::Script: {
    out.push_back(script);
    break;
  }
  case ConditionType::Vehicle: {
    out.push_back(vehicle.id);
    break;
  }
  default:
    break;
  }
}

std::string ConditionalBranchCommand::conditionalFormat(const std::string& text, const bool colored) const {
  return std::format("{}{}{}If{}{}{}", indentText(indent()), symbol(code()), ColorFormatter::getColorCode(code(), colored), colon, text, ColorFormatter::popColor(colored));
}

std::string ConditionalBranchCommand::stringRep(const Database& db, const bool colored) const {
  std::string strBuild;
  if (type == ConditionType::Variable) {
    std::string var = db.system.variable(variable.id);
    std::string other;
    if (variable.source == VariableComparisonSource::Variable) {
      other = db.variableNameOrId(variable.otherId);
    } else {
      other = std::to_string(variable.otherId);
    }
    var = var.empty() ? std::format("#{:04}", variable.id) : var;
    return conditionalFormat(std::format("{} {} {}", var, DecodeEnumName(variable.comparison), other), colored);
  }
  if (type == ConditionType::Switch) {
    // TL-NOTE: Equivalence, a == b
    return conditionalFormat(trFormat("{0} is {1}", db.switchNameOrId(globalSwitch.switchIdx), DecodeEnumName(globalSwitch.checkIfOn)), colored);
  }
  if (type == ConditionType::Self_Switch) {
    // TL-NOTE: Equivalence, a == b
    return conditionalFormat(trFormat("Self Switch {0} is {1}", selfSw, DecodeEnumName(selfSwitch.checkIfOn)), colored);
  }
  if (type == ConditionType::Timer) {
    std::string min;
    std::string sec;

    if (timer.sec > 59) {
      min = std::to_string(timer.sec / 60);
      sec = std::to_string(timer.sec % 60);
    } else {
      min = "0";
      sec = std::to_string(timer.sec);
    }
    // TL-NOTE: Comparison operator, minutes, seconds
    return conditionalFormat(trFormat("Timer {0} {1} min {1} sec", DecodeEnumName(timer.comparison), min, sec), colored);
  }
  if (type == ConditionType::Actor) {
    const auto act = db.actorNameOrId(actor.id);
    switch (actor.type) {
    case ActorConditionType::Name: {
      return conditionalFormat(trFormat("Name of {0} is {1}", act, name), colored);
    }
    case ActorConditionType::Class: {
      return conditionalFormat(trFormat("Class of {0} is {1}", act, db.classNameOrId(actor.checkId)), colored);
    }
    case ActorConditionType::Skill: {
      return conditionalFormat(trFormat("{0} has learned {1}", act, db.skillNameOrId(actor.checkId)), colored);
    }
    case ActorConditionType::Weapon: {
      return conditionalFormat(trFormat("{0} has equipped {1}", act, db.weaponNameOrId(actor.checkId)), colored);
    }
    case ActorConditionType::Armor: {
      return conditionalFormat(trFormat("{0} has equipped {1}", act, db.armorNameOrId(actor.checkId)), colored);
    }
    case ActorConditionType::State: {
      return conditionalFormat(trFormat("{0} is affected by {1}", act, db.stateNameOrId(actor.checkId)), colored);
    }
    case ActorConditionType::In_The_Party: {
      return conditionalFormat(trFormat("{0} is in the party", act), colored);
    }
    }
  }
  if (type == ConditionType::Enemy) {
    if (enemy.type == EnemyConditionType::State) {
      auto state = db.states.state(enemy.stateId);
      return conditionalFormat(trFormat("#{0} is affected by {1}", std::to_string(enemy.id + 1), db.stateNameOrId(enemy.stateId)), colored);
    }
    return conditionalFormat(trFormat("#{0} is appeared", std::to_string(enemy.id + 1)), colored);
  }
  if (type == ConditionType::Character) {
    std::string name = db.eventNameOrId(character.id);
    return conditionalFormat(trFormat("{0} is facing {1}", name, DecodeEnumName(character.facing)), colored);
  }
  if (type == ConditionType::Vehicle) {
    return conditionalFormat(trFormat("{0} is driven", DecodeEnumName(vehicle.id)), colored);
  }
  if (type == ConditionType::Gold) {
    return conditionalFormat(std::format("Gold {0} {1}", DecodeEnumName(gold.type), gold.value), colored);
  }
  if (type == ConditionType::Item) {
    return conditionalFormat(trFormat("Party has {0}", db.itemNameOrId(item.id)), colored);
  }
  if (type == ConditionType::Weapon) {
    std::string ret = conditionalFormat(trFormat("Party has {0}", db.weaponNameOrId(equip.equipId)), colored);
    if (equip.includeEquipment) {
      ret += ColorFormatter::getColor(FormatColor::Gray, colored) + db.parentheses(trNOOP("Include Equipment")) + ColorFormatter::popColor(colored);
    }
    return ret;
  }
  if (type == ConditionType::Armor) {
    std::string ret = conditionalFormat(trFormat("Party has {0}", db.armorNameOrId(equip.equipId)), colored);
    if (equip.includeEquipment) {
      ret += ColorFormatter::getColor(FormatColor::Gray, colored) + db.parentheses(trNOOP("Include Equipment")) + ColorFormatter::popColor(colored);
    }
    return ret;
  }
  if (type == ConditionType::Button) {
    return conditionalFormat(trFormat("Button [{0}] is pressed down", DecodeEnumName(button)), colored);
  }
  if (type == ConditionType::Script) {
    return conditionalFormat(tr("Script") + ": " + script, colored);
  }
  return indentText(indent()) + diamond.data() + (colored ? "&push-color=255,0,255;Condition&pop-color;&push-color=0,255,0;TBD&pop-color;" : "ConditionType TBD");
}
