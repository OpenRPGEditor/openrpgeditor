#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeClass.hpp"

struct Dialog_ChangeClass : IEventDialogController {
  Dialog_ChangeClass() = delete;
  explicit Dialog_ChangeClass(const std::string& name, const std::shared_ptr<ChangeClassCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeClassCommand());
    }
    m_actor = command->actor;
    m_class = command->classId;
    m_saveLevel = command->saveLevel;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

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