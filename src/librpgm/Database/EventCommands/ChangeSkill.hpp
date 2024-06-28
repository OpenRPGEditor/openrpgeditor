#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeSkillCommand : IEventCommand {
  ~ChangeSkillCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Skill; }

  ActorComparisonSource comparison;
  int value;
  SkillOperation skillOp;
  int skill;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Skill : {}, " + DecodeEnumName(skillOp) + " []" + ColorFormatter::popColor();
  }
};
