#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/EnemyAppear.hpp"

struct Project;
struct Dialog_EnemyAppear : IDialogController {
  Dialog_EnemyAppear() = delete;
  explicit Dialog_EnemyAppear(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new EnemyAppearCommand();
    m_troop_selection = command->enemy;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_troop_selection;

  bool m_confirmed{false};
EnemyAppearCommand* command;
  std::tuple<bool, bool> result;
};