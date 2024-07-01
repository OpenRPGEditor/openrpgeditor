#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/OpenMenu.hpp"

struct Project;
struct Dialog_OpenMenuScreen : IDialogController {
  Dialog_OpenMenuScreen() = delete;
  explicit Dialog_OpenMenuScreen(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<OpenMenuCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<OpenMenuCommand> command;
  std::tuple<bool, bool> result;
};