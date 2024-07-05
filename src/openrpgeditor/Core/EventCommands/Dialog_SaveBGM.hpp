#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/SaveBGM.hpp"

struct Project;
struct Dialog_SaveBGM : IDialogController {
  Dialog_SaveBGM() = delete;
  explicit Dialog_SaveBGM(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new SaveBGMCommand();
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{true};
SaveBGMCommand* command;
  std::tuple<bool, bool> result;
};