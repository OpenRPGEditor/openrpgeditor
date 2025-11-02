#pragma once
#include "Database/EventCommands/GetOnOffVehicle.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_GetOnOffVehicle : IEventDialogController {
  Dialog_GetOnOffVehicle() = delete;
  explicit Dialog_GetOnOffVehicle(const std::string& name, const std::shared_ptr<GetOnOffVehicleCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new GetOnOffVehicleCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<GetOnOffVehicleCommand> command;
  std::tuple<bool, bool> result;
};