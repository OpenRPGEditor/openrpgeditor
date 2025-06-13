#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/InputNumber.hpp"
#include "Database/EventCommands/SelectItem.hpp"

struct Dialog_SelectItem : IEventDialogController {
  Dialog_SelectItem() = delete;
  explicit Dialog_SelectItem(const std::string& name, const std::shared_ptr<SelectItemCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new SelectItemCommand());
    }
    m_variable = m_command->item;
    m_type = static_cast<int>(m_command->type);
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_variable;
  int m_type;

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<SelectItemCommand> m_command;
};