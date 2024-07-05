#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeSkillCommand final : IEventCommand {
  ~ChangeSkillCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Skill; }

  ActorComparisonSource comparison;
  int value;
  SkillOperation skillOp;
  int skill;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
