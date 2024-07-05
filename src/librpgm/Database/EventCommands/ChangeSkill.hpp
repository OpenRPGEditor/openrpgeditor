#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeSkillCommand final : IEventCommand {
  ChangeSkillCommand() = default;
  explicit ChangeSkillCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeSkillCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Skill; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison;
  int value;
  SkillOperation skillOp;
  int skill;
};
