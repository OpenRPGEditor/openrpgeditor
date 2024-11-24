#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangePlayerFollowers.hpp"

struct Dialog_ChangePlayerFollowers : IEventDialogController {
  Dialog_ChangePlayerFollowers() = delete;
  explicit Dialog_ChangePlayerFollowers(const std::string& name, const std::shared_ptr<ChangePlayerFollowersCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangePlayerFollowersCommand());
    }
    m_checkIfOn = static_cast<int>(command->followersEnabled);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_checkIfOn;
  bool m_confirmed{false};
  std::shared_ptr<ChangePlayerFollowersCommand> command;
  std::tuple<bool, bool> result;
};