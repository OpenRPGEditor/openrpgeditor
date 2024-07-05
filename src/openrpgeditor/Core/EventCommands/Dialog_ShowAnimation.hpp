#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/Animation.hpp"
#include "Database/Classes.hpp"
#include "Database/EventCommands/ChangeClass.hpp"
#include "Database/EventCommands/ShowAnimation.hpp"

struct Project;
struct Dialog_ShowAnimation : IDialogController {
  Dialog_ShowAnimation() = delete;
  explicit Dialog_ShowAnimation(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command = new ShowAnimationCommand();
    m_character = command->character;
    m_animation = command->animation;
    m_waitCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:

  bool m_waitCompletion;
  int m_animation;
  int m_character;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Animation>> animation_picker;
  ShowAnimationCommand* command;
  std::tuple<bool, bool> result;
};