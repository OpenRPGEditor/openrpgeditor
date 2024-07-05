#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/Classes.hpp"
#include "Database/EventCommands/ChangeClass.hpp"

struct Project;
struct Dialog_ChangeClass : IDialogController {
  Dialog_ChangeClass() = delete;
  explicit Dialog_ChangeClass(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new ChangeClassCommand());
    m_actor = command->actor;
    m_class = command->classId;
    m_saveLevel = command->saveLevel;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_actor;
  int m_class;
  bool m_saveLevel;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<ObjectPicker<Class>> class_picker;
  std::shared_ptr<ChangeClassCommand> command;
  std::tuple<bool, bool> result;
};