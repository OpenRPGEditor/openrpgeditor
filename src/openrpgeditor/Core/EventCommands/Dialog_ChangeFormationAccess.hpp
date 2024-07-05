#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeFormationAccess.hpp"

struct Project;
struct Dialog_ChangeFormationAccess : IDialogController {
  Dialog_ChangeFormationAccess() = delete;
  explicit Dialog_ChangeFormationAccess(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ChangeFormationAccessCommand();
    isDisabled = static_cast<int>(command->access);

  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int isDisabled;
  bool m_confirmed{false};
ChangeFormationAccessCommand* command;
  std::tuple<bool, bool> result;
};