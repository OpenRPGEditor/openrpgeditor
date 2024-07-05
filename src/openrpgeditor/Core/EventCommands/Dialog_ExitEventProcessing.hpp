#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ExitEventProcessing.hpp"

struct Project;
struct Dialog_ExitEventProcessing : IDialogController {
  Dialog_ExitEventProcessing() = delete;
  explicit Dialog_ExitEventProcessing(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ExitEventProcessingCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<ExitEventProcessingCommand> command;
  std::tuple<bool, bool> result;
};