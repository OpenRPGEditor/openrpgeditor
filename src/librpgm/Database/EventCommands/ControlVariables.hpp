#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlVariables : IEventCommand {
  ControlVariables() = default;
  explicit ControlVariables(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ControlVariables() override{};
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Variables; }
  int start{};
  int end{};
  VariableControlOperation operation{};
  VariableControlOperand operand{};
  union {
    int constant;
    int variable;
    struct {
      int min;
      int max;
    } random;
    struct {
      GameDataType type;
      int value;
      union {
        ActorDataSource actorSource;
        EnemyDataSource enemySource;
        CharacterDataSource characterSource;
        OtherDataSource otherSource;
        int rawSource;
      };
    } gameData;
    int raw{};
  };
  std::string script{};
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  [[nodiscard]] std::string variableFormat(const std::string& text) const;
};