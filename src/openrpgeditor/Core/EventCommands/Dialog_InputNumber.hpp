#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/InputNumber.hpp"

struct Project;
struct Dialog_InputNumber : IDialogController {
  Dialog_InputNumber() = delete;
  explicit Dialog_InputNumber(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new InputNumberCommand());
    m_variable = command->variable;
    m_digits = command->digits;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_variable;
  int m_digits;

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<InputNumberCommand> command;
  std::tuple<bool, bool> result;
};