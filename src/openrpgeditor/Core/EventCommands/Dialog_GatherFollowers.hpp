#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/GatherFollowers.hpp"

struct Project;
struct Dialog_GatherFollowers : IDialogController {
  Dialog_GatherFollowers() = delete;
  explicit Dialog_GatherFollowers(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<GatherFollowersCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<GatherFollowersCommand> command;
  std::tuple<bool, bool> result;
};