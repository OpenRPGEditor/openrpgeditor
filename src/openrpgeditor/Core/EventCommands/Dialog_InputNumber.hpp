#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/InputNumber.hpp"

struct Dialog_InputNumber : IEventDialogController {
  Dialog_InputNumber() = delete;
  explicit Dialog_InputNumber(const std::string& name, const std::shared_ptr<InputNumberCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new InputNumberCommand());
    }
    m_variable = command->variable;
    m_digits = command->digits;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_variable;
  int m_digits;

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<InputNumberCommand> command;
  std::tuple<bool, bool> result;
};