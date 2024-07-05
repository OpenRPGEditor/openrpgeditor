#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeBattleback.hpp"

struct Project;
struct Dialog_ChangeBattleBack : IDialogController {
  Dialog_ChangeBattleBack() = delete;
  explicit Dialog_ChangeBattleBack(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command = new ChangeBattlebackCommand();
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{false};
  ChangeBattlebackCommand* command;
  std::tuple<bool, bool> result;
};