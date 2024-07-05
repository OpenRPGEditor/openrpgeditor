#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NextScriptCommand final : IEventCommand {
  NextScriptCommand() = default;
  explicit NextScriptCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~NextScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Script; }
  std::string script;
};

struct ScriptCommand final : IEventCommand {
  ScriptCommand() = default;
  explicit ScriptCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  void addText(NextScriptCommand* text) { moreScript.emplace_back(text); }

  std::string script;
  std::vector<std::shared_ptr<NextScriptCommand>> moreScript;
};
