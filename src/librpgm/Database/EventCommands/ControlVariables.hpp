#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlVariables : IEventCommand {
  ControlVariables() = default;
  explicit ControlVariables(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ControlVariables() override {};
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Variables; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  [[nodiscard]] std::string variableFormat(const std::string& text) const;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ControlVariables>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      if (start == targetId) {
        return true;
      }
      return (variable == targetId && operand == VariableControlOperand::Variable);
    }
    return false;
  };

  int start{1};
  int end{1};
  VariableControlOperation operation = VariableControlOperation::_set__del_Set;
  VariableControlOperand operand = VariableControlOperand::Constant;
  union {
    int constant;
    int variable;
    struct {
      int min;
      int max;
    } random;
    struct {
      GameDataType type = GameDataType::Other;
      int value;
      union {
        ActorDataSource actorSource;
        EnemyDataSource enemySource;
        CharacterDataSource characterSource;
        OtherDataSource otherSource;
        int rawSource{0};
      };
    } gameData;
    int raw[4]{};
  };
  std::string script{};
};