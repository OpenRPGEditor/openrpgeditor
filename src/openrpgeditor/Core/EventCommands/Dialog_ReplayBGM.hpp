#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ResumeBGM.hpp"

struct Project;
struct Dialog_ReplayBGM : IDialogController {
  Dialog_ReplayBGM() = delete;
  explicit Dialog_ReplayBGM(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    m_open = true;
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ResumeBGMCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{true};
  std::optional<ResumeBGMCommand> command;
  std::tuple<bool, bool> result;
};