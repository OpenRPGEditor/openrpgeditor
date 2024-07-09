#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/EventCommands/ChangeItems.hpp"
#include "Database/Items.hpp"

struct Project;
struct Dialog_ChangeItems : IEventDialogController {
  Dialog_ChangeItems() = delete;
  explicit Dialog_ChangeItems(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeItemsCommand());

    m_item = static_cast<int>(command->item);
    m_operation = static_cast<int>(command->operation);
    m_operandSource = static_cast<int>(command->operandSource);

    if (command->operandSource == QuantityChangeSource::Variable)
      m_quantity_var = command->operand;
    else
      m_quantity = command->operand;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:


  int m_item;
  int m_operation;
  int m_operandSource;
  int m_quantity{1};
  int m_quantity_var{1};
  bool m_includeEquipment{false};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Item>> item_picker;
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<ChangeItemsCommand> command;
  std::tuple<bool, bool> result;
};