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
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<EnemyAppearCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int m_troop_selection = 0;

  bool m_confirmed{false};
  std::optional<EnemyAppearCommand> command;
  std::tuple<bool, bool> result;
};