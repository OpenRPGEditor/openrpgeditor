#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ResumeBGM.hpp"

struct Project;
struct Dialog_ReplayBGM : IDialogController {
  Dialog_ReplayBGM() = delete;
  explicit Dialog_ReplayBGM(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ResumeBGMCommand();
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
ResumeBGMCommand* command;
  std::tuple<bool, bool> result;
};