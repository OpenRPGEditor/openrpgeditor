#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Enemies.hpp"
#include "Database/EventCommands/EnemyTransform.hpp"

struct Dialog_EnemyTransform : IEventDialogController {
  Dialog_EnemyTransform() = delete;
  explicit Dialog_EnemyTransform(const std::string& name, const std::shared_ptr<EnemyTransformCommand>& cmd = nullptr, const int troopId = 0)
  : IEventDialogController(name)
  , m_troopId(troopId)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new EnemyTransformCommand());
    }
    m_enemy = m_command->enemy;
    m_troopMemberSelection = m_command->troopMember;
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_troopMemberSelection = 0;
  int m_enemy = 1;
  int m_troopId = 0;

  bool m_confirmed{false};
  std::shared_ptr<EnemyTransformCommand> m_command;
  std::optional<ObjectPicker<Enemy>> m_enemyPicker;
  std::tuple<bool, bool> result;
};