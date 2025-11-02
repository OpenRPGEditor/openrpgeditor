#pragma once
#include "Database/EventCommands/SetVehicleLocation.hpp"
#include "Database/EventCommands/TransferPlayer.hpp"
#include "Editor/CommonUI/EventTilePicker.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_SetVehicleLocation : IEventDialogController {
  Dialog_SetVehicleLocation() = delete;
  explicit Dialog_SetVehicleLocation(const std::string& name, const std::shared_ptr<SetVehicleLocationCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new SetVehicleLocationCommand());
    }
    m_mode = static_cast<int>(m_command->mode);
    m_vehicle = static_cast<int>(m_command->vehicle);
    if (m_command->mode == TransferMode::Variable_Designation) {
      m_mapIdVar = m_command->mapId;
      m_xVar = m_command->x;
      m_yVar = m_command->y;
    } else {
      m_mapId = m_command->mapId;
      m_x = m_command->x;
      m_y = m_command->y;
    }
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_vehicle{};
  int m_mode{};
  int m_mapId{};
  int m_x{};
  int m_y{};

  int m_mapIdVar{1};
  int m_xVar{1};
  int m_yVar{1};
  int m_variableSelection{};

  bool m_confirmed{false};
  std::shared_ptr<SetVehicleLocationCommand> m_command;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::optional<EventTilePicker> m_eventTilePicker;
};