#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/Label.hpp"

struct Dialog_Label : IEventDialogController {
  Dialog_Label() = delete;
  explicit Dialog_Label(const std::string& name, const std::shared_ptr<LabelCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new LabelCommand());
    }
    m_label = m_command->label;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  std::string m_label;

  bool m_confirmed{false};
  std::shared_ptr<LabelCommand> m_command;
};