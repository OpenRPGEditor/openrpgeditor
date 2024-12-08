#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/Animation.hpp"
#include "Database/EventCommands/ShowBattleAnimation.hpp"

struct Dialog_ShowBattleAnimation : IEventDialogController {
  Dialog_ShowBattleAnimation() = delete;
  explicit Dialog_ShowBattleAnimation(const std::string& name, const std::shared_ptr<ShowBattleAnimationCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShowBattleAnimationCommand());
    }
    m_enemy = command->enemy;
    m_animation = command->animation;
    m_targetAllEnemies = command->targetAllEnemies;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_targetAllEnemies;
  int m_animation;
  int m_enemy;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Animation>> animation_picker;
  std::shared_ptr<ShowBattleAnimationCommand> command;
  std::tuple<bool, bool> result;
};