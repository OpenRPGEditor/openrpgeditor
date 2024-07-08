#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/EnemyAppear.hpp"

struct Project;
struct Dialog_EnemyAppear : IEventDialogController {
  Dialog_EnemyAppear() = delete;
  explicit Dialog_EnemyAppear(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new EnemyAppearCommand());
    m_troop_selection = command->enemy;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_troop_selection;

  bool m_confirmed{false};
  std::shared_ptr<EnemyAppearCommand> command;
  std::tuple<bool, bool> result;
};