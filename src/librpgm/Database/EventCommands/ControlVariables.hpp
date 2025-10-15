#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlVariables : IEventCommand {
  ControlVariables() {};
  explicit ControlVariables(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ControlVariables() override {};
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Variables; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  [[nodiscard]] std::string variableFormat(const std::string& text, bool colored) const;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ControlVariables>(*this); }
  bool hasReference(const int targetId, const SearchType type) override {
    if (type == SearchType::Variable) {
      if (start == targetId && end == targetId) {
        return true;
      }
      if (operand == VariableControlOperand::Script) {
        const std::string constVal = Database::instance()->gameConstants->variables[targetId];
        if (constVal.empty()) {
          return false;
        }
        if (script.contains("gameVariables") && script.contains(constVal)) {
          return true;
        }
      }
      return (variable == targetId && operand == VariableControlOperand::Variable);
    }
    // ControlVariables can change switches by using scripts
    if (type == SearchType::Switch) {
      if (operand == VariableControlOperand::Script) {
        const std::string constVal = Database::instance()->gameConstants->switches[targetId];
        if (constVal.empty()) {
          return false;
        }
        if (script.contains("gameSwitches") && script.contains(constVal)) {
          return true;
        }
      }
    }
    if (type == SearchType::Range) {
      if (start != end) {
        return true;
      }
    }
    return false;
  };

  bool setReference(const int targetId, const int newId, const SearchType type) override {
    if (hasReference(targetId, type)) {
      if (start == targetId && end == targetId) {
        start = newId;
        end = newId;
      }
      return true;
    }
    return false;
  }

  int start{1};
  int end{1};
  VariableControlOperation operation = VariableControlOperation::_set__del_Set;
  VariableControlOperand operand = VariableControlOperand::Constant;
  int constant{0};
  int variable{0};
  struct {
    int min{0};
    int max{0};
  } random;

  struct {
    GameDataType type = GameDataType::Other;
    union {
      OtherDataSource otherSource;
      int value{0};
    };
    union {
      ActorDataSource actorSource;
      EnemyDataSource enemySource;
      CharacterDataSource characterSource;
      int rawSource{0};
    };
  } gameData;

  std::string script{};
  int m_troopId{0};
};