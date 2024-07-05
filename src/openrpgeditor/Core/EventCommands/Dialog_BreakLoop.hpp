#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/BreakLoop.hpp"

struct Project;
struct Dialog_BreakLoop : IDialogController {
  Dialog_BreakLoop() = delete;
  explicit Dialog_BreakLoop(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command = new BreakLoopCommand();
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  BreakLoopCommand* command;
  std::tuple<bool, bool> result;
};