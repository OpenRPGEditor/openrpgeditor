#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeSkillCommand final : IEventCommand {
  ChangeSkillCommand() = default;
  explicit ChangeSkillCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeSkillCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Skill; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeSkillCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return comparison == ActorComparisonSource::Variable && value == targetId;
    }
    if (type == SearchType::Skill) {
      return targetId == skill;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (comparison == ActorComparisonSource::Variable && value == targetId) {
        value = newId;
      }
      if (type == SearchType::Skill) {
        skill = newId;
      }
      return true;
    }
    return false;
  }
  ActorComparisonSource comparison = ActorComparisonSource::Fixed;
  int value{1};
  SkillOperation skillOp = SkillOperation::_plu__del_Learn;
  int skill{1};
};
