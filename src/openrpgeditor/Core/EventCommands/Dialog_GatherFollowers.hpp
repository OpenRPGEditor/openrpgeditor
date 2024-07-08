#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/GatherFollowers.hpp"

struct Project;
struct Dialog_GatherFollowers : IEventDialogController {
  Dialog_GatherFollowers() = delete;
  explicit Dialog_GatherFollowers(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new GatherFollowersCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<GatherFollowersCommand> command;
  std::tuple<bool, bool> result;
};