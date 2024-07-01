#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/StopSE.hpp"

struct Project;
struct Dialog_StopSE : IDialogController {
  Dialog_StopSE() = delete;
  explicit Dialog_StopSE(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<StopSECommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<StopSECommand> command;
  std::tuple<bool, bool> result;
};