#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeSaveAccess.hpp"

struct Project;
struct Dialog_ChangeSaveAccess : IDialogController {
  Dialog_ChangeSaveAccess() = delete;
  explicit Dialog_ChangeSaveAccess(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeSaveAccessCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{false};
  std::optional<ChangeSaveAccessCommand> command;
  std::tuple<bool, bool> result;
};