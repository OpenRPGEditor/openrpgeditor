#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/OpenSave.hpp"

struct Project;
struct Dialog_OpenSaveScreen : IDialogController {
  Dialog_OpenSaveScreen() = delete;
  explicit Dialog_OpenSaveScreen(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<OpenSaveCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<OpenSaveCommand> command;
  std::tuple<bool, bool> result;
};