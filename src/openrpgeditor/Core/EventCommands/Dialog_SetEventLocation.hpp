#pragma once
#include "Core/CommonUI/EventTilePicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/SetEventLocation.hpp"

struct Dialog_SetEventLocation : IEventDialogController {
  Dialog_SetEventLocation() = delete;
  explicit Dialog_SetEventLocation(const std::string& name, const std::shared_ptr<SetEventLocationCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new SetEventLocationCommand());
    }
    m_mode = static_cast<int>(m_command->mode);
    m_event = m_command->event;

    if (m_command->mode == TransferMode::Variable_Designation) {
      m_xVar = m_command->x;
      m_yVar = m_command->y;
    } else if (m_command->mode == TransferMode::Exchange_With_Another_Event) {
      m_otherEvent = m_command->x;
      m_yVar = 0;
    } else {
      m_x = m_command->x;
      m_y = m_command->y;
    }
    m_direction = static_cast<int>(m_command->direction);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  // If exchanging, it uses x_var for event id
  int m_event{};
  int m_mode{};
  int m_x{};
  int m_y{};
  int m_direction{};

  int m_xVar{1};
  int m_yVar{1};
  int m_otherEvent{0};
  int m_variableSelection{};

  bool m_confirmed{false};
  std::shared_ptr<SetEventLocationCommand> m_command;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::optional<EventTilePicker> m_eventTilePicker;
};