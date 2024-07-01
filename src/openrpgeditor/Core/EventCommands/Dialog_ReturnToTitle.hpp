#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ReturnToTitle.hpp"

struct Project;
struct Dialog_ReturnToTitle : IDialogController {
  Dialog_ReturnToTitle() = delete;
  explicit Dialog_ReturnToTitle(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ReturnToTitleCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::optional<ReturnToTitleCommand> command;
  std::tuple<bool, bool> result;
};