#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/CommandParser.hpp"
struct Project;
struct Dialog_ControlVariables : IDialogController {
  Dialog_ControlVariables() = delete;
  explicit Dialog_ControlVariables(const char* name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
    command->start = 0;
    command->end = 0;
    command->random.min = 0;
    command->random.max = 0;
  }
  void draw() override;
  Project* m_project = nullptr;
private:
  int d_start = 0;
  int d_end = 0;
  int d_constant = 0;
  int d_variable = 0;
  int d_rand_1 = 0;
  int d_rand_2 = 0;
  std::string script = "";

  std::optional<ControlVariables> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
  bool singleRequest = false;
};