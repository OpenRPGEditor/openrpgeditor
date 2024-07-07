#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangePlayerFollowers.hpp"

struct Project;
struct Dialog_ChangePlayerFollowers : IDialogController {
  Dialog_ChangePlayerFollowers() = delete;
  explicit Dialog_ChangePlayerFollowers(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new ChangePlayerFollowersCommand());
    m_checkIfOn = static_cast<int>(command->followersEnabled);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_checkIfOn;
  bool m_confirmed{false};
  std::shared_ptr<ChangePlayerFollowersCommand> command;
  std::tuple<bool, bool> result;
};