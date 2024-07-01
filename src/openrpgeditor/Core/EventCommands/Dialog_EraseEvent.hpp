#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/EraseEvent.hpp"

struct Project;
struct Dialog_EraseEvent : IDialogController {
  Dialog_EraseEvent() = delete;
  explicit Dialog_EraseEvent(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<EraseEventCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<EraseEventCommand> command;
  std::tuple<bool, bool> result;
};