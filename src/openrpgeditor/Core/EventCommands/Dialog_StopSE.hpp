#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/StopSE.hpp"

struct Project;
struct Dialog_StopSE : IDialogController {
  Dialog_StopSE() = delete;
  explicit Dialog_StopSE(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new StopSECommand();
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
StopSECommand* command;
  std::tuple<bool, bool> result;
};