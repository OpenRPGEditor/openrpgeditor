#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/EventCommands/MovementRoute/Frequency.hpp"
#include "Database/EventCommands/SetEventLocation.hpp"
#include "Database/EventCommands/SetMovementRoute.hpp"

struct Dialog_SetMovementRoute : IEventDialogController {
  Dialog_SetMovementRoute() = delete;
  explicit Dialog_SetMovementRoute(const std::string& name, const std::shared_ptr<SetMovementRouteCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new SetMovementRouteCommand());
    }
    m_character = command->character;
    m_route = command->route;
    m_editNodes = command->editNodes;
    if (m_route.list.empty()) {
      m_route.addCommand(std::make_shared<EventDummy>(), 0);
    }
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_character;
  int m_selected{0};

  MovementRoute m_route;
  std::vector<std::shared_ptr<MovementRouteStepCommand>> m_editNodes;

  bool m_confirmed{false};
  std::shared_ptr<SetMovementRouteCommand> command;
  std::shared_ptr<IEventDialogController> movementRouteDialog;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};