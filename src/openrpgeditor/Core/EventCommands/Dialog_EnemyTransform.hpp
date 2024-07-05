#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Enemies.hpp"
#include "Database/EventCommands/EnemyTransform.hpp"

struct Project;
struct Dialog_EnemyTransform : IDialogController {
  Dialog_EnemyTransform() = delete;
  explicit Dialog_EnemyTransform(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<EnemyTransformCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int m_troop_selection = 0;
  int m_enemy = 1;

  bool m_confirmed{false};
  std::optional<EnemyTransformCommand> command;
  std::optional<ObjectPicker<Enemy>> enemy_picker;
  std::tuple<bool, bool> result;
};