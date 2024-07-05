#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/AbortBattle.hpp"

struct Project;
struct Dialog_AbortBattle : IDialogController {
  Dialog_AbortBattle() = delete;
  explicit Dialog_AbortBattle(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new AbortBattleCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<AbortBattleCommand> command;
  std::tuple<bool, bool> result;
};