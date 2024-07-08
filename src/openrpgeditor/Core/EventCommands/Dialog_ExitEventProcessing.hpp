#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ExitEventProcessing.hpp"

struct Project;
struct Dialog_ExitEventProcessing : IEventDialogController {
  Dialog_ExitEventProcessing() = delete;
  explicit Dialog_ExitEventProcessing(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ExitEventProcessingCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<ExitEventProcessingCommand> command;
  std::tuple<bool, bool> result;
};