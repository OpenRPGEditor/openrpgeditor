#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/EraseEvent.hpp"

struct Project;
struct Dialog_EraseEvent : IDialogController {
  Dialog_EraseEvent() = delete;
  explicit Dialog_EraseEvent(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new EraseEventCommand();
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  EraseEventCommand* command;
  std::tuple<bool, bool> result;
};