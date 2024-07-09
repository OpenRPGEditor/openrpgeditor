#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/SetEventLocation.hpp"
#include "Database/EventCommands/SetMovementRoute.hpp"

struct Project;
struct Dialog_SetMovementRoute : IEventDialogController {
  Dialog_SetMovementRoute() = delete;
  explicit Dialog_SetMovementRoute(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
     command.reset(new SetMovementRouteCommand());
     m_character = command->character;
     m_route = command->route;
     m_editNodes = command->editNodes;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_character;
  int m_selected{0};

  MovementRoute m_route;
  std::vector<std::shared_ptr<MovementRouteStepCommand>> m_editNodes;

  bool m_confirmed{false};
  std::shared_ptr<SetMovementRouteCommand> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};