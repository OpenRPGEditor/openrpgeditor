#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ScrollMap.hpp"

struct Dialog_ScrollMap : IEventDialogController {
  Dialog_ScrollMap() = delete;
  explicit Dialog_ScrollMap(const std::string& name, const std::shared_ptr<ScrollMapCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ScrollMapCommand());
    }

    m_direction = static_cast<int>(command->direction);
    m_distance = command->distance;
    m_speed = static_cast<int>(command->speed);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_direction;
  int m_distance;
  int m_speed;

  bool m_confirmed{false};
  std::shared_ptr<ScrollMapCommand> command;
  std::tuple<bool, bool> result;
};