#pragma once

#include "Core/EventCommands/IDialogController.hpp"

#include <memory>
#include <vector>

struct IEventCommand;
struct Project;
struct EventCommandEditor {
  EventCommandEditor(Project* project) : m_project(project), commandDialog(nullptr) {}
  void draw();

  void setCommands(std::vector<std::shared_ptr<IEventCommand>>* commands) {
    m_commands = commands;
    m_selectedCommand = 0;
  }
private:
  void drawPopup(std::shared_ptr<IEventCommand> command, IDialogController* controller);
  Project* m_project = nullptr;
  bool m_isNewEntry{false};
  int m_selectedCommand = 0;
  IDialogController* commandDialog;
  std::vector<std::shared_ptr<IEventCommand>>* m_commands = nullptr;
};
