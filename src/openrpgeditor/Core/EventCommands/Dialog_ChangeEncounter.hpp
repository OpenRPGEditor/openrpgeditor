#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeEncounterDisable.hpp"

struct Project;
struct Dialog_ChangeEncounter : IDialogController {
  Dialog_ChangeEncounter() = delete;
  explicit Dialog_ChangeEncounter(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ChangeEncounterDisableCommand();
    isDisabled = static_cast<int>(command->access);
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{false};
ChangeEncounterDisableCommand* command;
  std::tuple<bool, bool> result;
};