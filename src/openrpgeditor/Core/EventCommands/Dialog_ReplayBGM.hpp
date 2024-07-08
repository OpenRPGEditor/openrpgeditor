#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ResumeBGM.hpp"

struct Project;
struct Dialog_ReplayBGM : IEventDialogController {
  Dialog_ReplayBGM() = delete;
  explicit Dialog_ReplayBGM(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new ResumeBGMCommand());
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
  std::shared_ptr<ResumeBGMCommand> command;
  std::tuple<bool, bool> result;
};