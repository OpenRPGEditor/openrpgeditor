#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeFormationAccess.hpp"

struct Project;
struct Dialog_ChangeFormationAccess : IEventDialogController {
  Dialog_ChangeFormationAccess() = delete;
  explicit Dialog_ChangeFormationAccess(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeFormationAccessCommand());
    isDisabled = static_cast<int>(command->access);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int isDisabled;
  bool m_confirmed{false};
  std::shared_ptr<ChangeFormationAccessCommand> command;
  std::tuple<bool, bool> result;
};