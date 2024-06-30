#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/Script.hpp"

struct Project;
struct Dialog_Script : IDialogController {
  Dialog_Script() = delete;
  explicit Dialog_Script(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ScriptCommand>(command.value()); };
  //std::shared_ptr<std::vector<std::optional<IEventCommand>>> getNextCommands() override {
  //  return std::make_shared<std::vector<IEventCommand>>(nextScripts);
  //};

  Project* m_project = nullptr;

private:
  std::string script;
  bool m_confirmed{false};
  std::optional<ScriptCommand> command;
  std::vector<NextScriptCommand> nextScripts;
  std::tuple<bool, bool> result;
};

inline std::vector<std::string> splitString(const std::string& str, char delimiter) {
  std::vector<std::string> tokens;
  std::istringstream ss(str);
  std::string token;

  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}