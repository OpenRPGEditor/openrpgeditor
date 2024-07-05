#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/GetOnOffVehicle.hpp"

struct Project;
struct Dialog_GetOnOffVehicle : IDialogController {
  Dialog_GetOnOffVehicle() = delete;
  explicit Dialog_GetOnOffVehicle(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new GetOnOffVehicleCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<GetOnOffVehicleCommand> command;
  std::tuple<bool, bool> result;
};