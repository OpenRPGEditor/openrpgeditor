#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ExitEventProcessing.hpp"

struct Project;
struct Dialog_ExitEventProcessing : IDialogController {
  Dialog_ExitEventProcessing() = delete;
  explicit Dialog_ExitEventProcessing(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ExitEventProcessingCommand();
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
ExitEventProcessingCommand* command;
  std::tuple<bool, bool> result;
};