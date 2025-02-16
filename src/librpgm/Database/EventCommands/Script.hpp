#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NextScriptCommand final : IEventCommand {
  NextScriptCommand() = default;
  explicit NextScriptCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~NextScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Script; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<NextScriptCommand>(*this); }

  std::string script;
};

struct ScriptCommand final : IEventCommand {
  ScriptCommand() = default;
  explicit ScriptCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ScriptCommand>(*this); }

  void addText(NextScriptCommand* text) { moreScript.emplace_back(text); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Variable) {
      if (script.contains("gameVariables") && script.contains(Database::instance()->gameConstants.variables[targetId])) {
        return true;
      }
    }
    if (type == SearchType::Switch) {
      if (script.contains("gameSwitches") && script.contains(Database::instance()->gameConstants.switches[targetId])) {
        return true;
      }
    }
    for (auto& nextCmd : moreScript) {
      if (type == SearchType::Variable) {
        if (nextCmd->script.contains("gameVariables") && script.contains(Database::instance()->gameConstants.variables[targetId])) {
          return true;
        }
      }
      if (type == SearchType::Switch) {
        if (nextCmd->script.contains("gameSwitches") && script.contains(Database::instance()->gameConstants.switches[targetId])) {
          return true;
        }
      }
    }
    return false;
  };
  bool hasStringReference(std::string text, SearchType type) {
    if (text == "") {
      return true;
    }
    if (script.contains(text)) {
      return true;
    }
    for (auto& nextCmd : moreScript) {
      if (nextCmd->script.contains(text)) {
        return true;
      }
    }
    return false;
  }
  std::string script;
  std::vector<std::shared_ptr<NextScriptCommand>> moreScript;
};
