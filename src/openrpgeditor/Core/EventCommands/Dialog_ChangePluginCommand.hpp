#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/Plugin.hpp"

struct Project;
struct Dialog_ChangePluginCommand : IEventDialogController {
  Dialog_ChangePluginCommand() = delete;
  explicit Dialog_ChangePluginCommand(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new PluginCommandMV());
    m_command = command->command;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  std::string m_command;

  bool m_confirmed{false};
  std::shared_ptr<PluginCommandMV> command;
  std::tuple<bool, bool> result;
};