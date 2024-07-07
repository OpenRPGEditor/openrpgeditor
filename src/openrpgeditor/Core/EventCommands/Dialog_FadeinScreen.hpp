#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/FadeinScreen.hpp"

struct Project;
struct Dialog_FadeinScreen : IDialogController {
  Dialog_FadeinScreen() = delete;
  explicit Dialog_FadeinScreen(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new FadeinScreenCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<FadeinScreenCommand> command;
  std::tuple<bool, bool> result;
};