#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeMenuAccess.hpp"

struct Project;
struct Dialog_ChangeMenuAccess : IDialogController {
  Dialog_ChangeMenuAccess() = delete;
  explicit Dialog_ChangeMenuAccess(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeMenuAccessCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{false};
  std::optional<ChangeMenuAccessCommand> command;
  std::tuple<bool, bool> result;
};