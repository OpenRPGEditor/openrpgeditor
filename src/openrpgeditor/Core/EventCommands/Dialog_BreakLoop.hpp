#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/BreakLoop.hpp"

struct Project;
struct Dialog_BreakLoop : IDialogController {
  Dialog_BreakLoop() = delete;
  explicit Dialog_BreakLoop(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<BreakLoopCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<BreakLoopCommand> command;
  std::tuple<bool, bool> result;
};