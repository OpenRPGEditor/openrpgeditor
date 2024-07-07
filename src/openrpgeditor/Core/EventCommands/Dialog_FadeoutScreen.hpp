#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/FadeoutScreen.hpp"

struct Project;
struct Dialog_FadeoutScreen : IDialogController {
  Dialog_FadeoutScreen() = delete;
  explicit Dialog_FadeoutScreen(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new FadeoutScreenCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<FadeoutScreenCommand> command;
  std::tuple<bool, bool> result;
};