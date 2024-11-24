#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Enemies.hpp"
#include "Database/EventCommands/EnemyTransform.hpp"

struct Dialog_EnemyTransform : IEventDialogController {
  Dialog_EnemyTransform() = delete;
  explicit Dialog_EnemyTransform(const std::string& name, const std::shared_ptr<EnemyTransformCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new EnemyTransformCommand());
    }
    m_enemy = command->transform;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_troop_selection = 0;
  int m_enemy = 1;

  bool m_confirmed{false};
  std::shared_ptr<EnemyTransformCommand> command;
  std::optional<ObjectPicker<Enemy>> enemy_picker;
  std::tuple<bool, bool> result;
};