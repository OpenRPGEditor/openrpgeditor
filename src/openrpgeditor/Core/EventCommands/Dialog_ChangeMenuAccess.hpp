#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeMenuAccess.hpp"

struct Project;
struct Dialog_ChangeMenuAccess : IDialogController {
  Dialog_ChangeMenuAccess() = delete;
  explicit Dialog_ChangeMenuAccess(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new ChangeMenuAccessCommand());
    isDisabled = static_cast<int>(command->access);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int isDisabled;
  bool m_confirmed{false};
  std::shared_ptr<ChangeMenuAccessCommand> command;
  std::tuple<bool, bool> result;
};