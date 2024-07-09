#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/TransferPlayer.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/SetVehicleLocation.hpp"

struct Project;
struct Dialog_SetVehicleLocation : IEventDialogController {
  Dialog_SetVehicleLocation() = delete;
  explicit Dialog_SetVehicleLocation(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new SetVehicleLocationCommand());
    m_mode = static_cast<int>(command->mode);
    m_vehicle = static_cast<int>(command->vehicle);
    if (command->mode == TransferMode::Variable_Designation) {
      m_mapId_var = command->mapId;
      m_x_var = command->x;
      m_y_var = command->y;
    }
    else {
      m_mapId = command->mapId;
      m_x = command->x;
      m_y = command->y;
    }
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_vehicle;
  int m_mode;
  int m_mapId;
  int m_x;
  int m_y;

  int m_mapId_var{1};
  int m_x_var{1};
  int m_y_var{1};
  int m_var_selection;

  bool m_confirmed{false};
  std::shared_ptr<SetVehicleLocationCommand> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};