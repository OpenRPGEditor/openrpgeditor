#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ControlSwitches.hpp"

struct Dialog_ControlSwitches : IEventDialogController {
  Dialog_ControlSwitches() = delete;
  explicit Dialog_ControlSwitches(const std::string& name, const std::shared_ptr<ControlSwitches>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ControlSwitches());
    }
    m_start = command->start;
    m_end = command->end;
    m_isDisabled = static_cast<int>(command->turnOff);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  void drawPickers();

  int m_start;
  int m_end;
  int m_rangeMin{1};
  int m_rangeMax{1};
  int m_isDisabled;
  int m_operation{0};

  std::shared_ptr<ControlSwitches> command;
  std::optional<VariableSwitchPicker> picker;
};