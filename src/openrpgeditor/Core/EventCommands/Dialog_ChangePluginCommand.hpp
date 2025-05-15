#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/Plugin.hpp"

struct Dialog_ChangePluginCommand : IEventDialogController {
  Dialog_ChangePluginCommand() = delete;
  explicit Dialog_ChangePluginCommand(const std::string& name, const std::shared_ptr<PluginCommandMV>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new PluginCommandMV());
    }
    m_pluginText = m_command->command;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  std::string m_pluginText;

  bool m_confirmed{false};
  std::shared_ptr<PluginCommandMV> m_command;
};