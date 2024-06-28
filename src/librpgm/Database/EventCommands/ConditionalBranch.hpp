#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ConditionalBranchCommand : IEventCommand {
  ~ConditionalBranchCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Conditional_Branch; }
  ConditionType type{};
  struct {
    int switchIdx{};
    ValueControl checkIfOn;
  } globalSwitch{};
  struct {
    int id{};
    VariableComparisonSource source{};
    VariableComparisonType comparison{};
    union {
      int constant;
      int otherId{};
    };
  } variable{};
  struct {
    ValueControl checkIfOn;
  } selfSwitch{};
  struct {
    TimerComparisonType comparison{};
    int sec{};
  } timer{};
  struct {
    int id{};
    ActorConditionType type{};
    int checkId{};
  } actor{};
  struct {
    int id{};
    EnemyConditionType type{};
    int stateId{}; // For "State" condition
  } enemy;
  struct {
    int id{};
    Direction facing{};
  } character{};
  struct {
    int id{};
  } vehicle;
  struct {
    GoldComaprisonType type{};
    int value{};
  } gold{};
  struct {
    int id{};
  } item{};
  /* For Weapon and Armor conditions */
  struct {
    int equipId{};
    bool includeEquipment{};
  } equip{};
  std::string button;
  std::string selfSw; // A, B, C, D
  std::string name;
  std::string script;

  [[nodiscard]] std::string stringRep() const override {
    std::string strBuild;
    if (type == ConditionType::Variable) {
      std::string test = ColorFormatter::getColorCode(code());
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} " + DecodeEnumName(variable.comparison) + " " +
             (variable.source == VariableComparisonSource::Constant ? std::to_string(variable.constant) : "{}");
    }
    if (type == ConditionType::Switch) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If " + "{}" + " is " +
             (globalSwitch.checkIfOn == ValueControl::OFF ? "OFF" : "ON");
    }
    if (type == ConditionType::Self_Switch) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If Self Switch " + "{}" + " is " +
             (selfSwitch.checkIfOn == ValueControl::OFF ? "OFF" : "ON");
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
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If Timer" + ColorFormatter::popColor() + " " + DecodeEnumName(timer.comparison) + " " + min + " min " +
             sec + " sec";
    }
    if (type == ConditionType::Actor) {
      if (actor.type == ActorConditionType::Name) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If Name of {} is []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::Class) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If Class of {} is []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::Skill) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} has learned []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::Weapon || actor.type == ActorConditionType::Armor) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} has equipped []" + ColorFormatter::popColor();
      } else if (actor.type == ActorConditionType::State) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} is affected by []" + ColorFormatter::popColor();
      } else {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} is in party" + ColorFormatter::popColor();
      }
    }
    if (type == ConditionType::Enemy) {
      if (enemy.type == EnemyConditionType::State) {
        return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
     + "If {} is affected by []" + ColorFormatter::popColor();
      }
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "If {} is appeared" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Character) {

      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
             + "If {} is facing " + DecodeEnumName(character.facing) + ColorFormatter::popColor();
    }
    if (type == ConditionType::Vehicle) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
              + "If {} is driven" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Gold) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
               + "If Gold" + " " + DecodeEnumName<GoldComaprisonType>(gold.type) + " {}";
    }
    if (type == ConditionType::Item) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
              + "If Party has {}" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Weapon || type == ConditionType::Armor) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
              + "If Party has {}" + ColorFormatter::popColor() + ColorFormatter::getColor(Color::Gray) +
             " (Include Equipment)" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Button) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
             + "If Button [" + "{}" + "] is pressed down" + ColorFormatter::popColor();
    }
    if (type == ConditionType::Script) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
             + "If Script : " + ColorFormatter::popColor();
    }
    return std::string(indent ? *indent * 4 : 0, ' ') +
           "◇ &push-color=255,0,255;Condition&pop-color; &push-color=0,255,0;TBD&pop-color;";
  }
};

struct ElseCommand : IEventCommand {
  ~ElseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Else; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Else"; }
};