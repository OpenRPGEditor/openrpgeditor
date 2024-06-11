#pragma once

#include <memory>
#include <vector>

struct IEventCommand;
struct Project;
struct EventCommandEditor {
  EventCommandEditor(Project* project) : m_project(project) {}
  void draw();

  void setCommands(std::vector<std::shared_ptr<IEventCommand>>* commands) {
    m_commands = commands;
  }
private:
  Project* m_project = nullptr;
  std::vector<std::shared_ptr<IEventCommand>>* m_commands = nullptr;
};
