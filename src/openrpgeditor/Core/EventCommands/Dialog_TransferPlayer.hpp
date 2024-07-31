#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/TransferPlayer.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"

struct Dialog_TransferPlayer : IEventDialogController {
  Dialog_TransferPlayer() = delete;
  explicit Dialog_TransferPlayer(const std::string& name,
                                 const std::shared_ptr<TransferPlayerCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new TransferPlayerCommand());
    }
    m_mode = static_cast<int>(command->mode);

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
    m_direction = static_cast<int>(command->direction);
    m_fade = static_cast<int>(command->fade);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:

  int m_mode;
  int m_mapId;
  int m_x;
  int m_y;
  int m_direction;
  int m_fade;

  int m_mapId_var{1};
  int m_x_var{1};
  int m_y_var{1};
  int m_var_selection;

  bool m_confirmed{false};
  std::shared_ptr<TransferPlayerCommand> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};