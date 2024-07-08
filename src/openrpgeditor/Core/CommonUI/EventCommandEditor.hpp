#pragma once

#include "Core/EventCommands/IEventDialogController.hpp"

#include <memory>
#include <vector>

struct IEventCommand;
struct Project;
struct EventCommandEditor {
  EventCommandEditor(Project* project) : m_project(project), commandDialog(nullptr) {}
  void draw();

  void setCommands(std::vector<std::shared_ptr<IEventCommand>>* commands) {
    m_commands = commands;
    m_selectedCommand = -1;
  }
private:
  void drawPopup();
  Project* m_project = nullptr;
  bool m_isNewEntry{false};
  int m_selectedCommand = -1;
  std::shared_ptr<IEventDialogController> commandDialog;
  std::vector<std::shared_ptr<IEventCommand>>* m_commands = nullptr;
};
