#pragma once
#include "Database/EventCommands/JumpToLabel.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_JumpToLabel : IEventDialogController {
  Dialog_JumpToLabel() = delete;
  explicit Dialog_JumpToLabel(const std::string& name, const std::shared_ptr<JumpToLabelCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new JumpToLabelCommand());
    }
    m_label = m_command->label;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  std::string m_label;

  bool m_confirmed{false};
  std::shared_ptr<JumpToLabelCommand> m_command;
};