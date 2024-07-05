#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/OpenMenu.hpp"

struct Project;
struct Dialog_OpenMenuScreen : IDialogController {
  Dialog_OpenMenuScreen() = delete;
  explicit Dialog_OpenMenuScreen(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new OpenMenuCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<OpenMenuCommand> command;
  std::tuple<bool, bool> result;
};