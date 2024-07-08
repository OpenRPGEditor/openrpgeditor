#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/Wait.hpp"

struct Project;
struct Dialog_Wait : IEventDialogController {
  Dialog_Wait() = delete;
  explicit Dialog_Wait(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new WaitCommand());
    m_waitDuration = command->duration;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_waitDuration;
  bool m_confirmed{false};
  std::shared_ptr<WaitCommand> command;
  std::tuple<bool, bool> result;
};