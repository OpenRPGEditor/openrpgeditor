#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/OpenSave.hpp"

struct Project;
struct Dialog_OpenSaveScreen : IDialogController {
  Dialog_OpenSaveScreen() = delete;
  explicit Dialog_OpenSaveScreen(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new OpenSaveCommand());
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<OpenSaveCommand> command;
  std::tuple<bool, bool> result;
};