#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/EventCommands/Label.hpp"

struct Dialog_Label : IEventDialogController {
  Dialog_Label() = delete;
  explicit Dialog_Label(const std::string& name,
                        const std::shared_ptr<LabelCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new LabelCommand());
    }
    m_label = command->label;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  std::string m_label;

  bool m_confirmed{false};
  std::shared_ptr<LabelCommand> command;
  std::tuple<bool, bool> result;
};