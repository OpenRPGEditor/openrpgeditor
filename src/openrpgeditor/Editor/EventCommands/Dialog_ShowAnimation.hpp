#pragma once
#include "Database/Animation.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeClass.hpp"
#include "Database/EventCommands/ShowAnimation.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ShowAnimation : IEventDialogController {
  Dialog_ShowAnimation() = delete;
  explicit Dialog_ShowAnimation(const std::string& name, const std::shared_ptr<ShowAnimationCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ShowAnimationCommand());
    }
    m_character = m_command->character;
    m_animation = m_command->animation;
    m_waitCompletion = m_command->waitForCompletion;
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  bool m_waitCompletion;
  int m_animation;
  int m_character;

  std::optional<AnimationPicker> m_animationPicker;
  std::shared_ptr<ShowAnimationCommand> m_command;
};