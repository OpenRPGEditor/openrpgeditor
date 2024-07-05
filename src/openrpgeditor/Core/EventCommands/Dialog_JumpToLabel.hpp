#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/EventCommands/Label.hpp"

struct Project;
struct Dialog_JumpToLabel : IDialogController {
  Dialog_JumpToLabel() = delete;
  explicit Dialog_JumpToLabel(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new LabelCommand());
    m_label = command->label;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  std::string m_label;

  bool m_confirmed{false};
  std::shared_ptr<LabelCommand> command;
  std::tuple<bool, bool> result;
};