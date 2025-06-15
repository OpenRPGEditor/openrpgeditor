#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Animation.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ShowBattleAnimation.hpp"

struct Dialog_ShowBattleAnimation : IEventDialogController {
  Dialog_ShowBattleAnimation() = delete;
  explicit Dialog_ShowBattleAnimation(const std::string& name, const std::shared_ptr<ShowBattleAnimationCommand>& cmd = nullptr, const int troopId = 0)
  : IEventDialogController(name)
  , m_troop(troopId)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ShowBattleAnimationCommand());
    }
    m_enemy = m_command->enemy;
    m_animation = m_command->animation;
    m_targetAllEnemies = m_command->targetAllEnemies;
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_troop{0};
  bool m_targetAllEnemies;
  int m_animation;
  int m_enemy;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Animation>> m_animationPicker;
  std::shared_ptr<ShowBattleAnimationCommand> m_command;
};