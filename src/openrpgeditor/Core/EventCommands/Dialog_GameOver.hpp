#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/GameOver.hpp"

struct Project;
struct Dialog_GameOver : IDialogController {
  Dialog_GameOver() = delete;
  explicit Dialog_GameOver(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new GameOverCommand();
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
GameOverCommand* command;
  std::tuple<bool, bool> result;
};