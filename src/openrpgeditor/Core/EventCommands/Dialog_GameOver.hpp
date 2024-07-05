#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/GameOver.hpp"

struct Project;
struct Dialog_GameOver : IDialogController {
  Dialog_GameOver() = delete;
  explicit Dialog_GameOver(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new GameOverCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;
  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<GameOverCommand> command;
  std::tuple<bool, bool> result;
};