#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/GetOnOffVehicle.hpp"

struct Project;
struct Dialog_GetOnOffVehicle : IDialogController {
  Dialog_GetOnOffVehicle() = delete;
  explicit Dialog_GetOnOffVehicle(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<GetOnOffVehicleCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<GetOnOffVehicleCommand> command;
  std::tuple<bool, bool> result;
};