#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/Loop.hpp"

struct Project;
struct Dialog_Loop : IEventDialogController {
  Dialog_Loop() = delete;
  explicit Dialog_Loop(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new LoopCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<LoopCommand> command;
  std::tuple<bool, bool> result;
};