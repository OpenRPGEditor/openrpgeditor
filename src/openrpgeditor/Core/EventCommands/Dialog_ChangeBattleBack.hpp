#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeBattleback.hpp"

struct Project;
struct Dialog_ChangeBattleBack : IDialogController {
  Dialog_ChangeBattleBack() = delete;
  explicit Dialog_ChangeBattleBack(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeBattlebackCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{false};
  std::optional<ChangeBattlebackCommand> command;
  std::tuple<bool, bool> result;
};