#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlVariables : IEventCommand {
  ControlVariables() = default;
  explicit ControlVariables(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ControlVariables() override{};
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Variables; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  [[nodiscard]] std::string variableFormat(const std::string& text) const;
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