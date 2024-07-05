#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ReturnToTitle.hpp"

struct Project;
struct Dialog_ReturnToTitle : IDialogController {
  Dialog_ReturnToTitle() = delete;
  explicit Dialog_ReturnToTitle(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new ReturnToTitleCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  std::shared_ptr<ReturnToTitleCommand> command;
  std::tuple<bool, bool> result;
};