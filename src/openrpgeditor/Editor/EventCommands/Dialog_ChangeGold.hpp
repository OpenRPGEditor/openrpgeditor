#pragma once
#include "Database/EventCommands/ChangeGold.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ChangeGold : IEventDialogController {
  Dialog_ChangeGold() = delete;
  explicit Dialog_ChangeGold(const std::string& name, const std::shared_ptr<ChangeGoldCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeGoldCommand());
    }
    m_operation = static_cast<int>(m_command->operation);
    m_operandSource = static_cast<int>(m_command->operandSource);
    m_constant = m_command->operand;
    m_variable = m_command->operand;
  }

  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_operation;
  int m_operandSource;

  int m_constant;
  int m_variable;

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<ChangeGoldCommand> m_command;
};