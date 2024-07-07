#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeMapDisplayName.hpp"

struct Project;
struct Dialog_ChangeMapDisplayName : IDialogController {
  Dialog_ChangeMapDisplayName() = delete;
  explicit Dialog_ChangeMapDisplayName(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new ChangeMapNameDisplayCommand());
    m_checkIfOn = static_cast<int>(command->checkIfOn);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_checkIfOn;
  bool m_confirmed{false};
  std::shared_ptr<ChangeMapNameDisplayCommand> command;
  std::tuple<bool, bool> result;
};