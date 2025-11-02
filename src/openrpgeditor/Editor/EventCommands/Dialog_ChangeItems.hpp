#pragma once
#include "Database/EventCommands/ChangeItems.hpp"
#include "Database/Items.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ChangeItems : IEventDialogController {
  Dialog_ChangeItems() = delete;
  explicit Dialog_ChangeItems(const std::string& name, const std::shared_ptr<ChangeItemsCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeItemsCommand());
    }

    m_item = static_cast<int>(command->item);
    m_operation = static_cast<int>(command->operation);
    m_operandSource = static_cast<int>(command->operandSource);

    if (command->operandSource == QuantityChangeSource::Variable)
      m_quantityVar = command->operand;
    else
      m_quantity = command->operand;
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_item;
  int m_operation;
  int m_operandSource;
  int m_quantity{1};
  int m_quantityVar{1};
  bool m_includeEquipment{false};

  bool m_confirmed{false};
  std::optional<ItemPicker> m_itemPicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<ChangeItemsCommand> command;
  std::tuple<bool, bool> result;
};