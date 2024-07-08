#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/EnemyRecoverAll.hpp"
#include "Database/EventCommands/RecoverAll.hpp"

struct Project;
struct Dialog_EnemyRecoverAll : IEventDialogController {
  Dialog_EnemyRecoverAll() = delete;
  explicit Dialog_EnemyRecoverAll(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new EnemyRecoverAllCommand());
    m_troop_selection = command->troop;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_troop_selection = 0;

  bool m_confirmed{false};
  std::shared_ptr<EnemyRecoverAllCommand> command;
  std::tuple<bool, bool> result;
};