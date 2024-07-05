#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/EnemyRecoverAll.hpp"
#include "Database/EventCommands/RecoverAll.hpp"

struct Project;
struct Dialog_EnemyRecoverAll : IDialogController {
  Dialog_EnemyRecoverAll() = delete;
  explicit Dialog_EnemyRecoverAll(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new EnemyRecoverAllCommand();
    m_troop_selection = command->troop;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:

  int m_troop_selection = 0;

  bool m_confirmed{false};
EnemyRecoverAllCommand* command;
  std::tuple<bool, bool> result;
};