#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/Animation.hpp"
#include "Database/EventCommands/ShowBattleAnimation.hpp"

struct Project;
struct Dialog_ShowBattleAnimation : IEventDialogController {
  Dialog_ShowBattleAnimation() = delete;
  explicit Dialog_ShowBattleAnimation(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ShowBattleAnimationCommand());
    m_enemy = command->enemy;
    m_animation = command->animation;
    m_targetAllEnemies = command->targetAllEnemies;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_targetAllEnemies;
  int m_animation;
  int m_enemy;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Animation>> animation_picker;
  std::shared_ptr<ShowBattleAnimationCommand> command;
  std::tuple<bool, bool> result;
};