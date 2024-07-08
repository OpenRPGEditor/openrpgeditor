#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/InputNumber.hpp"
#include "Database/EventCommands/SelectItem.hpp"

struct Project;
struct Dialog_SelectItem : IEventDialogController {
  Dialog_SelectItem() = delete;
  explicit Dialog_SelectItem(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new SelectItemCommand());
    m_variable = command->item;
    m_type = static_cast<int>(command->type);

  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_variable;
  int m_type;

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<SelectItemCommand> command;
  std::tuple<bool, bool> result;
};