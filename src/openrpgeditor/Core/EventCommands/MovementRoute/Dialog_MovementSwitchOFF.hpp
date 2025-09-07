#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/SwitchOFF.hpp"

struct MainWindow;
struct Dialog_MovementSwitchOFF : IEventDialogController {
  Dialog_MovementSwitchOFF() = delete;
  explicit Dialog_MovementSwitchOFF(const std::string& name, const std::shared_ptr<MovementSwitchOFFCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new MovementSwitchOFFCommand());
    }
    m_picker.emplace(VariableSwitchPicker::Type::Switch,  Database::instance()->system.switches(), m_command->id);
    m_picker->setOpen(true);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

  MainWindow* m_project = nullptr;

private:
  bool m_confirmed{false};
  std::shared_ptr<MovementSwitchOFFCommand> m_command;
  std::optional<VariableSwitchPicker> m_picker;
};