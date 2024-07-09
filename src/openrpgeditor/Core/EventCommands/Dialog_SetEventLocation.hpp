#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/SetEventLocation.hpp"

struct Project;
struct Dialog_SetEventLocation : IEventDialogController {
  Dialog_SetEventLocation() = delete;
  explicit Dialog_SetEventLocation(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new SetEventLocationCommand());
    m_mode = static_cast<int>(command->mode);
    m_event = static_cast<int>(command->event);

    if (command->mode == TransferMode::Variable_Designation) {
      m_x_var = command->x;
      m_y_var = command->y;
    }
    else if (command->mode == TransferMode::Exchange_With_Another_Event) {
      m_otherEvent = command->x;
      m_y_var = 0;
    }
    else {
      m_x = command->x;
      m_y = command->y;
    }
    m_direction = static_cast<int>(command->direction);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:

  // If exchanging, it uses x_var for event id
  int m_event;
  int m_mode;
  int m_x;
  int m_y;
  int m_direction;

  int m_x_var{1};
  int m_y_var{1};
  int m_otherEvent{0};
  int m_var_selection;

  bool m_confirmed{false};
  std::shared_ptr<SetEventLocationCommand> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};