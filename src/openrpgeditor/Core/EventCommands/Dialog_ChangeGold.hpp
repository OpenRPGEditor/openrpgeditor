#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeGold.hpp"

struct Dialog_ChangeGold : IEventDialogController {
  Dialog_ChangeGold() = delete;
  explicit Dialog_ChangeGold(const std::string& name, const std::shared_ptr<ChangeGoldCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeGoldCommand());
    }
    m_operation = static_cast<int>(command->operation);
    m_operandSource = static_cast<int>(command->operandSource);
    m_constant = command->operand;
    m_variable = command->operand;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_operation;
  int m_operandSource;

  int m_constant;
  int m_variable;

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<ChangeGoldCommand> command;
  std::tuple<bool, bool> result;
};