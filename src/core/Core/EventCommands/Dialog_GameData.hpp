#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/CommandParser.hpp"
struct Project;
struct Dialog_GameData : IDialogController {
  Dialog_GameData() = delete;
  explicit Dialog_GameData(const char* name, Project* project) : IDialogController(name), m_project(project) {}
  void draw() override;

  Project* m_project = nullptr;
  ControlVariables command;
private:
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};