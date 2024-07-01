#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/GameOver.hpp"

struct Project;
struct Dialog_GameOver : IDialogController {
  Dialog_GameOver() = delete;
  explicit Dialog_GameOver(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<GameOverCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<GameOverCommand> command;
  std::tuple<bool, bool> result;
};