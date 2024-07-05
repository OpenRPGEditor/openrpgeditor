#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/GatherFollowers.hpp"

struct Project;
struct Dialog_GatherFollowers : IDialogController {
  Dialog_GatherFollowers() = delete;
  explicit Dialog_GatherFollowers(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new GatherFollowersCommand();
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
GatherFollowersCommand* command;
  std::tuple<bool, bool> result;
};