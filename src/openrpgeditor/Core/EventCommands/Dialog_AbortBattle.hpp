#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/AbortBattle.hpp"

struct Project;
struct Dialog_AbortBattle : IDialogController {
  Dialog_AbortBattle() = delete;
  explicit Dialog_AbortBattle(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<AbortBattleCommand>(); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<AbortBattleCommand> command;
  std::tuple<bool, bool> result;
};