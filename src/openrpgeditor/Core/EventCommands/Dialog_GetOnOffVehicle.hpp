#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/GetOnOffVehicle.hpp"

struct Dialog_GetOnOffVehicle : IEventDialogController {
  Dialog_GetOnOffVehicle() = delete;
  explicit Dialog_GetOnOffVehicle(const std::string& name,
                                  const std::shared_ptr<GetOnOffVehicleCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new GetOnOffVehicleCommand());
    }
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<GetOnOffVehicleCommand> command;
  std::tuple<bool, bool> result;
};