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
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<EnemyRecoverAllCommand>(command.value()); };
  Project* m_project = nullptr;

private:

  int m_troop_selection = 0;

  bool m_confirmed{false};
  std::optional<EnemyRecoverAllCommand> command;
  std::tuple<bool, bool> result;
};