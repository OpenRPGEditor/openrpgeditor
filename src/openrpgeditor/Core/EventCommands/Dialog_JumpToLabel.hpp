#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/JumpToLabel.hpp"

struct Dialog_JumpToLabel : IEventDialogController {
  Dialog_JumpToLabel() = delete;
  explicit Dialog_JumpToLabel(const std::string& name, const std::shared_ptr<JumpToLabelCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new JumpToLabelCommand());
    }
    m_label = command->label;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  std::string m_label;

  bool m_confirmed{false};
  std::shared_ptr<JumpToLabelCommand> command;
  std::tuple<bool, bool> result;
};