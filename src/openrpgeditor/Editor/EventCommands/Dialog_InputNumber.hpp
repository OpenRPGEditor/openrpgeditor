#pragma once
#include "Database/EventCommands/InputNumber.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_InputNumber : IEventDialogController {
  Dialog_InputNumber() = delete;
  explicit Dialog_InputNumber(const std::string& name, const std::shared_ptr<InputNumberCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new InputNumberCommand());
    }
    m_variable = m_command->variable;
    m_digits = m_command->digits;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_variable;
  int m_digits;

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<InputNumberCommand> m_command;
};