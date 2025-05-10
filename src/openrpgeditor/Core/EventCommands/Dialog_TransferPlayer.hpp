#pragma once
#include "Core/CommonUI/EventTilePicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/TransferPlayer.hpp"

struct Dialog_TransferPlayer : IEventDialogController {
  Dialog_TransferPlayer() = delete;
  explicit Dialog_TransferPlayer(const std::string& name, const std::shared_ptr<TransferPlayerCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new TransferPlayerCommand());
    }
    m_mode = static_cast<int>(m_command->mode);

    if (m_command->mode == TransferMode::Variable_Designation) {
      m_mapIdVar = m_command->mapId;
      m_xVar = m_command->x;
      m_yVar = m_command->y;
    } else {
      m_mapId = m_command->mapId;
      m_x = m_command->x;
      m_y = m_command->y;
    }
    m_direction = static_cast<int>(m_command->direction);
    m_fade = static_cast<int>(m_command->fade);
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_mode;
  int m_mapId;
  int m_x;
  int m_y;
  int m_direction;
  int m_fade;

  int m_mapIdVar{1};
  int m_xVar{1};
  int m_yVar{1};
  int m_varSelection;

  bool m_confirmed{false};
  std::shared_ptr<TransferPlayerCommand> m_command;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::optional<EventTilePicker> m_eventTilePicker;
};