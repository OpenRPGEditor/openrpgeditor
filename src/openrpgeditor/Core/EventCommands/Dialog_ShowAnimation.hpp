#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Animation.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeClass.hpp"
#include "Database/EventCommands/ShowAnimation.hpp"

struct Dialog_ShowAnimation : IEventDialogController {
  Dialog_ShowAnimation() = delete;
  explicit Dialog_ShowAnimation(const std::string& name, const std::shared_ptr<ShowAnimationCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShowAnimationCommand());
    }
    m_character = command->character;
    m_animation = command->animation;
    m_waitCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_waitCompletion;
  int m_animation;
  int m_character;
  ImVec2 windowSize{183, 141};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Animation>> animation_picker;
  std::shared_ptr<ShowAnimationCommand> command;
  std::tuple<bool, bool> result;
};