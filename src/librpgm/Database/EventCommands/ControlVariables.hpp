#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlVariables : IEventCommand {
  ControlVariables() {}
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
      GameDataSource source;
      union {
        ActorDataSource actorSource;
        EnemyDataSource enemySource;
        CharacterDataSource characterSource;
        OtherDataSource otherSource;
        int rawSource;
      };
      int value;
    } gameData;
    int raw{};
  };
  std::string script{};
  [[nodiscard]] std::string stringRep() const override {
    std::string strBuild;
    if (start != end) {
      strBuild = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Variables : #" + std::format("{:04}", start) + "..#" + std::format("{:04}", end) + " " +
                 DecodeEnumName(operation) + " ";
    } else {
      strBuild = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Variables : #" + std::format("{:04}", start) + " {} " + DecodeEnumName(operation) + " ";
    }

    if (operand == VariableControlOperand::Random) {
      strBuild += "Random " + std::to_string(random.min) + ".." + std::to_string(random.max);
    } else if (operand == VariableControlOperand::Variable) {
      strBuild += std::format("{:04}", variable) + " {}";
    } else if (operand == VariableControlOperand::Game_Data) {
      if (gameData.source == GameDataSource::Character) {
        strBuild += DecodeEnumName(static_cast<CharacterDataSource>(gameData.value)) + " of " +
                    (gameData.rawSource == -1  ? "Player"
                     : gameData.rawSource == 0 ? "This Event"
                                               : "{}");
      } else if (gameData.source == GameDataSource::Party) {
        strBuild += "Actor ID of the party member #" + std::to_string(gameData.rawSource);
      } else if (gameData.source == GameDataSource::Other) {
        strBuild += DecodeEnumName(static_cast<OtherDataSource>(gameData.value));
      } else {
        strBuild += "The number of {}";
      }
    } else {
      if (operand == VariableControlOperand::Script)
        strBuild += script;
      else
        strBuild += std::to_string(constant);
    }
    return strBuild + ColorFormatter::popColor();
  }
};