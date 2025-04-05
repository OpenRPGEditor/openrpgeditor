#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/Plugin.hpp"

struct Dialog_ChangePluginCommand : IEventDialogController {
  Dialog_ChangePluginCommand() = delete;
  explicit Dialog_ChangePluginCommand(const std::string& name, const std::shared_ptr<PluginCommandMV>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new PluginCommandMV());
    }
    strncpy(m_command, command->command.c_str(), 4096);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  char m_command[4096];

  bool m_confirmed{false};
  std::shared_ptr<PluginCommandMV> command;
  std::tuple<bool, bool> result;
};