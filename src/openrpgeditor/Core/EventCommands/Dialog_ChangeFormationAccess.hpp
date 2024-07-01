#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeFormationAccess.hpp"

struct Project;
struct Dialog_ChangeFormationAccess : IDialogController {
  Dialog_ChangeFormationAccess() = delete;
  explicit Dialog_ChangeFormationAccess(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeFormationAccessCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{false};
  std::optional<ChangeFormationAccessCommand> command;
  std::tuple<bool, bool> result;
};