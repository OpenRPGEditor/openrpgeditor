#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeStateCommand final : IEventCommand {
  ChangeStateCommand() = default;
  explicit ChangeStateCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeStateCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_State; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeStateCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      return comparison == ActorComparisonSource::Variable && value == targetId;
    }
    if (type == SearchType::State) {
      return targetId == state;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (comparison == ActorComparisonSource::Variable && value == targetId) {
        value = newId;
      }
      if (type == SearchType::State) {
        state = newId;
      }
      return true;
    }
    return false;
  }
  ActorComparisonSource comparison = ActorComparisonSource::Fixed;
  int value{1};
  PartyMemberOperation stateOp = PartyMemberOperation::_plu__del_Add;
  int state{1};
};