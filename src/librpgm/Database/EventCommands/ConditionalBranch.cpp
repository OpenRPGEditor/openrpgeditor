#include "Database/EventCommands/ConditionalBranch.hpp"

#include "Database/Database.hpp"

std::string ConditionalBranchCommand::conditionalFormat(const std::string& text) const {
  return std::format("{}{}{}If{}{}{}", indentText(indent), symbol(code()), ColorFormatter::getColorCode(code()), colon,
                     text, ColorFormatter::popColor());
}

std::string ConditionalBranchCommand::stringRep(const Database& db) const {
  std::string strBuild;
  ;
  if (type == ConditionType::Variable) {
    std::string var = db.system.variable(variable.id);
    std::string other;
    if (variable.source == VariableComparisonSource::Variable) {
      other = db.system.variable(variable.otherId);
      other = other.empty() ? std::format("#{:04}", variable.otherId) : other;
    } else {
      other = std::to_string(variable.otherId);
    }
    var = var.empty() ? std::format("#{:04}", variable.id) : var;
    return conditionalFormat(std::format("{} {} {}", var, DecodeEnumName(variable.comparison), other));
  }
  if (type == ConditionType::Switch) {
    std::string sw = db.system.switche(globalSwitch.switchIdx);
    sw = sw.empty() ? std::format("#{:04}", globalSwitch.switchIdx) : sw;
    return conditionalFormat(std::format("{} is {}", sw, DecodeEnumName(globalSwitch.checkIfOn)));
  }
  if (type == ConditionType::Self_Switch) {
    return conditionalFormat(std::format("Self Switch {} is {}", selfSw, DecodeEnumName(selfSwitch.checkIfOn)));
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
    return conditionalFormat(std::format("Timer {} {} min {} sec", DecodeEnumName(timer.comparison), min, sec));
  }
  if (type == ConditionType::Actor) {
    const auto act = db.actors.actor(actor.id);
    switch (actor.type) {
    case ActorConditionType::Name: {
      return conditionalFormat(std::format("Name of {} is {}", act->name, name));
    }
    case ActorConditionType::Class: {
      const auto cls = db.classes.classType(actor.checkId);
      return conditionalFormat(std::format("Class of {} is {}", act->name, cls->name));
    }
    case ActorConditionType::Skill: {
      const auto skill = db.skills.skill(actor.checkId);
      return conditionalFormat(std::format("{} has learned {}", act->name, skill->name));
    }
    case ActorConditionType::Weapon: {
      const auto weapon = db.weapons.weapon(actor.checkId);
      return conditionalFormat(std::format("{} has equipped {}", act->name, weapon->name));
    }
    case ActorConditionType::Armor: {
      const auto armor = db.armors.armor(actor.checkId);
      return conditionalFormat(std::format("{} has equipped {}", act->name, armor->name));
    }
    case ActorConditionType::State: {
      const auto state = db.states.state(actor.checkId);
      return conditionalFormat(std::format("{} is affected by {}", act->name, state->name));
    }
    case ActorConditionType::In_The_Party: {
      return conditionalFormat(std::format("{} is in the party", act->name));
    }
    }
  }
  if (type == ConditionType::Enemy) {
    if (enemy.type == EnemyConditionType::State) {
      auto state = db.states.state(enemy.stateId);
      return conditionalFormat(std::format("#{} is affected by {}", std::to_string(enemy.id + 1), state->name));
    }
    return conditionalFormat(std::format("#{} is appeared", std::to_string(enemy.id + 1)));
  }
  if (type == ConditionType::Character) {
    const auto map = db.mapInfos.currentMap();
    const Event* event = map != nullptr ? map->event(character.id) : nullptr;
    std::string name = character.id == -1    ? "Player"
                       : character.id == 0   ? "This Event"
                       : event->name.empty() ? std::format("Unnamed Event #{:03}", character.id)
                                             : event->name;
    return conditionalFormat(std::format("{} is facing {}", name, DecodeEnumName(character.facing)));
  }
  if (type == ConditionType::Vehicle) {
    return conditionalFormat(std::format("{} is driven", DecodeEnumName(vehicle.id)));
  }
  if (type == ConditionType::Gold) {
    return conditionalFormat(std::format("Gold {} {}", DecodeEnumName(gold.type), gold.value));
  }
  if (type == ConditionType::Item) {
    const auto itm = db.items.item(item.id);
    return conditionalFormat(std::format("Party has {}", itm->name));
  }
  if (type == ConditionType::Weapon) {
    std::string ret = conditionalFormat(std::format("Party has {}", db.weapons.weapon(equip.equipId)->name));
    if (equip.includeEquipment) {
      ret += ColorFormatter::getColor(Color::Gray) + "(Include Equipment)" + ColorFormatter::popColor();
    }
    return ret;
  }
  if (type == ConditionType::Armor) {
    std::string ret = conditionalFormat(std::format("Party has {}", db.armors.armor(equip.equipId)->name));
    if (equip.includeEquipment) {
      ret += ColorFormatter::getColor(Color::Gray) + "(Include Equipment)" + ColorFormatter::popColor();
    }
    return ret;
  }
  if (type == ConditionType::Button) {
    return conditionalFormat(std::format("Button [{}] is pressed down", DecodeEnumName(button)));
  }
  if (type == ConditionType::Script) {
    return conditionalFormat(std::format("Script: {}", script));
  }
  return std::string(indent ? *indent * 4 : 0, ' ') +
         "◇ &push-color=255,0,255;Condition&pop-color; &push-color=0,255,0;TBD&pop-color;";
}
