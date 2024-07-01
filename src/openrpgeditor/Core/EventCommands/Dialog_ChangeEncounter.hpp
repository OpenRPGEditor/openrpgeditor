#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeEncounterDisable.hpp"

struct Project;
struct Dialog_ChangeEncounter : IDialogController {
  Dialog_ChangeEncounter() = delete;
  explicit Dialog_ChangeEncounter(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeEncounterDisableCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int isDisabled = 0;
  bool m_confirmed{false};
  std::optional<ChangeEncounterDisableCommand> command;
  std::tuple<bool, bool> result;
};