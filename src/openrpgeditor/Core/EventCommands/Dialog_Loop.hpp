#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/Loop.hpp"

struct Project;
struct Dialog_Loop : IDialogController {
  Dialog_Loop() = delete;
  explicit Dialog_Loop(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<LoopCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<LoopCommand> command;
  std::tuple<bool, bool> result;
};