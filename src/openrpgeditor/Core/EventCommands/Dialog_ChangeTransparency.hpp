#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeTransparency.hpp"

struct Project;
struct Dialog_ChangeTransparency : IDialogController {
  Dialog_ChangeTransparency() = delete;
  explicit Dialog_ChangeTransparency(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new ChangeTransparencyCommand());
    isDisabled = static_cast<int>(command->transparency);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int isDisabled;
  bool m_confirmed{false};
  std::shared_ptr<ChangeTransparencyCommand> command;
  std::tuple<bool, bool> result;
};