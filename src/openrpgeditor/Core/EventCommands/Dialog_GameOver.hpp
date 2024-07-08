#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/GameOver.hpp"

struct Project;
struct Dialog_GameOver : IEventDialogController {
  Dialog_GameOver() = delete;
  explicit Dialog_GameOver(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
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