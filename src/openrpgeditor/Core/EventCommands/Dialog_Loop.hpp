#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/Loop.hpp"

struct Project;
struct Dialog_Loop : IDialogController {
  Dialog_Loop() = delete;
  explicit Dialog_Loop(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new LoopCommand();
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
LoopCommand* command;
  std::tuple<bool, bool> result;
};