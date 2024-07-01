#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/SaveBGM.hpp"

struct Project;
struct Dialog_SaveBGM : IDialogController {
  Dialog_SaveBGM() = delete;
  explicit Dialog_SaveBGM(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<SaveBGMCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<SaveBGMCommand> command;
  std::tuple<bool, bool> result;
};