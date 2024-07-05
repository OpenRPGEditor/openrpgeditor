#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeMenuAccess.hpp"

struct Project;
struct Dialog_ChangeMenuAccess : IDialogController {
  Dialog_ChangeMenuAccess() = delete;
  explicit Dialog_ChangeMenuAccess(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ChangeMenuAccessCommand();
    isDisabled = static_cast<int>(command->access);
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int isDisabled;
  bool m_confirmed{false};
ChangeMenuAccessCommand* command;
  std::tuple<bool, bool> result;
};