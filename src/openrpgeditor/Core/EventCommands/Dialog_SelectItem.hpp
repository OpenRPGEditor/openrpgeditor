#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/InputNumber.hpp"
#include "Database/EventCommands/SelectItem.hpp"

struct Dialog_SelectItem : IEventDialogController {
  Dialog_SelectItem() = delete;
  explicit Dialog_SelectItem(const std::string& name, const std::shared_ptr<SelectItemCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new SelectItemCommand());
    }
    m_variable = command->item;
    m_type = static_cast<int>(command->type);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_variable;
  int m_type;

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<SelectItemCommand> command;
  std::tuple<bool, bool> result;
};