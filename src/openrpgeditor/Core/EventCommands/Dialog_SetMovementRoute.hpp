#pragma once
#include <utility>

#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/EventCommands/MovementRoute/Frequency.hpp"
#include "Database/EventCommands/SetEventLocation.hpp"
#include "Database/EventCommands/SetMovementRoute.hpp"
#include "Database/EventPage.hpp"

struct Dialog_SetMovementRoute : IEventDialogController {
  Dialog_SetMovementRoute() = delete;
  explicit Dialog_SetMovementRoute(const std::string& name, const std::shared_ptr<SetMovementRouteCommand>& cmd = nullptr) : IEventDialogController(name), m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new SetMovementRouteCommand());
    }
    m_character = m_command->character;
    m_route = m_command->route;
    m_editNodes = m_command->editNodes;
    if (m_route.list().empty()) {
      m_route.addCommand(std::make_shared<EventDummy>(), 0);
    }
  }
  explicit Dialog_SetMovementRoute(const std::string& name, EventPage* page) : IEventDialogController(name), m_page(page) {
    m_isEventRoute = true;
    m_route = m_page->moveRoute();
    if (m_route.list().empty()) {
      m_route.addCommand(std::make_shared<EventDummy>(), 0);
    }
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

  void setPage(EventPage* page) {
    m_isEventRoute = true;
    m_route = m_page->moveRoute();
    if (m_route.list().empty()) {
      m_route.addCommand(std::make_shared<EventDummy>(), 0);
    }
  }

private:
  int m_character{-1};
  int m_selected{0};

  EventPage* m_page = nullptr; // For event routes only
  MovementRoute m_route;
  std::vector<std::shared_ptr<MovementRouteStepCommand>> m_editNodes;

  bool m_confirmed{false};
  bool m_isEventRoute{false};
  std::shared_ptr<SetMovementRouteCommand> m_command;
  std::shared_ptr<IEventDialogController> m_movementRouteDialog;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};