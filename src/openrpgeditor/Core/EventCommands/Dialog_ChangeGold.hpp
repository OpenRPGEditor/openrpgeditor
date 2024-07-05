#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/ChangeGold.hpp"

struct Project;
struct Dialog_ChangeGold : IDialogController {
  Dialog_ChangeGold() = delete;
  explicit Dialog_ChangeGold(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeGoldCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int m_operation = 0;
  int m_operandSource = 0;

  int m_constant = 1;
  int m_variable = 1;

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> picker;
  std::optional<ChangeGoldCommand> command;
  std::tuple<bool, bool> result;
};