#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeSkillCommand final : IEventCommand {
  ChangeSkillCommand() = default;
  explicit ChangeSkillCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeSkillCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Skill; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison;
  int value;
  SkillOperation skillOp;
  int skill;
};
