#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/EventCommands/Label.hpp"

struct Project;
struct Dialog_Label : IDialogController {
  Dialog_Label() = delete;
  explicit Dialog_Label(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new LabelCommand();
    m_label = command->label;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  std::string m_label;

  bool m_confirmed{false};
LabelCommand* command;
  std::tuple<bool, bool> result;
};