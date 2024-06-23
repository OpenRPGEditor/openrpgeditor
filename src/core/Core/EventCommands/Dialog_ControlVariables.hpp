#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/CommandParser.hpp"
struct Project;
struct Dialog_ControlVariables : IDialogController {
  Dialog_ControlVariables() = delete;
  explicit Dialog_ControlVariables(const char* name, Project* project) : IDialogController(name), m_project(project) {
    command.start = 0;
    command.end = 0;
    command.random.min = 0;
    command.random.max = 0;
  }
  void draw() override;

  ControlVariables command;
  Project* m_project = nullptr;
private:
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
  bool singleRequest = false;
};