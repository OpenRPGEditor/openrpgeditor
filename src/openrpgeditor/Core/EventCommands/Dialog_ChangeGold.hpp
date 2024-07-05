#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/ChangeGold.hpp"

struct Project;
struct Dialog_ChangeGold : IDialogController {
  Dialog_ChangeGold() = delete;
  explicit Dialog_ChangeGold(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new ChangeGoldCommand());
    m_operation = static_cast<int>(command->operation);
    m_operandSource = static_cast<int>(command->operandSource);
    m_constant = command->operand;
    m_variable = command->operand;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

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