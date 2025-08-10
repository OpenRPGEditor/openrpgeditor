#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ScrollMap.hpp"

struct Dialog_ScrollMap : IEventDialogController {
  Dialog_ScrollMap() = delete;
  explicit Dialog_ScrollMap(const std::string& name, const std::shared_ptr<ScrollMapCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ScrollMapCommand());
    }

    m_direction = static_cast<int>(m_command->direction);
    m_distance = m_command->distance;
    m_speed = static_cast<int>(m_command->speed);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_direction;
  int m_distance;
  int m_speed;

  bool m_confirmed{false};
  std::shared_ptr<ScrollMapCommand> m_command;
};