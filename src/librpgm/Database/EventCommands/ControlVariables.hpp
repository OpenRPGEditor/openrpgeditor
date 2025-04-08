#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlVariables : IEventCommand {
  ControlVariables() = default;
  explicit ControlVariables(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ControlVariables() override {};
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Variables; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  [[nodiscard]] std::string variableFormat(const std::string& text, bool colored) const;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ControlVariables>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      if (start == targetId && end == targetId) {
        return true;
      }
      if (operand == VariableControlOperand::Script) {
        std::string cnst = Database::instance()->gameConstants.variables[targetId];
        if (cnst.empty()) {
          return false;
        }
        if (script.contains("gameVariables") && script.contains(cnst)) {
          return true;
        }
      }
      return (variable == targetId && operand == VariableControlOperand::Variable);
    }
    // ControlVariables can change switches by using scripts
    if (type == SearchType::Switch) {
      if (operand == VariableControlOperand::Script) {
        std::string cnst = Database::instance()->gameConstants.switches[targetId];
        if (cnst.empty()) {
          return false;
        }
        if (script.contains("gameSwitches") && script.contains(cnst)) {
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

  bool setReference(int targetId, int newId, SearchType type) override {
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