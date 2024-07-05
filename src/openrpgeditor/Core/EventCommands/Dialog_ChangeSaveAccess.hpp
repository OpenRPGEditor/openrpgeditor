#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeSaveAccess.hpp"

struct Project;
struct Dialog_ChangeSaveAccess : IDialogController {
  Dialog_ChangeSaveAccess() = delete;
  explicit Dialog_ChangeSaveAccess(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new ChangeSaveAccessCommand());
    isDisabled = static_cast<int>(command->access);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int isDisabled;
  bool m_confirmed{false};
  std::shared_ptr<ChangeSaveAccessCommand> command;
  std::tuple<bool, bool> result;
};