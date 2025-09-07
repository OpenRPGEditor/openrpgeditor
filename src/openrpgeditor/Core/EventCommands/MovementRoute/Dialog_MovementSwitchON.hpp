#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/SwitchON.hpp"

struct MainWindow;
struct Dialog_MovementSwitchON : IEventDialogController {
  Dialog_MovementSwitchON() = delete;
  explicit Dialog_MovementSwitchON(const std::string& name, const std::shared_ptr<MovementSwitchONCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new MovementSwitchONCommand());
    }
    m_picker.emplace(VariableSwitchPicker::Type::Switch,  Database::instance()->system.switches(), m_command->id);
    m_picker->setOpen(true);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

  MainWindow* m_project = nullptr;

private:
  bool m_confirmed{false};
  std::shared_ptr<MovementSwitchONCommand> m_command;
  std::optional<VariableSwitchPicker> m_picker;
};