#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NextScriptCommand final : IEventCommand {
  ~NextScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Script; }
  std::string script;
};

struct ScriptCommand final : IEventCommand {
  ~ScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script; }
  std::string script;
  std::vector<std::shared_ptr<NextScriptCommand>> moreScript;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
