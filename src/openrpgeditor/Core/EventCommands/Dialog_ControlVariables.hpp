#pragma once
#include "Dialog_GameData.hpp"
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/CommandParser.hpp"
struct Project;
struct Dialog_ControlVariables : IDialogController {
  Dialog_ControlVariables() = delete;
  explicit Dialog_ControlVariables(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
    command->start = 0;
    command->end = 0;
    command->random.min = 0;
    command->random.max = 0;
  }
  std::tuple<bool, bool>  draw() override;
  Project* m_project = nullptr;

  std::shared_ptr<IEventCommand> getCommand() override {
    return std::make_shared<ControlVariables>(command.value());
  };

private:
  int m_start = 1;
  int m_end = 1;
  int m_constant = 0;
  int m_variable = 1;
  int m_rand_1 = 0;
  int m_rand_2 = 0;
  std::string script = "";

  std::optional<ControlVariables> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
  bool singleRequest = false;
  std::optional<Dialog_GameData> gameDataDialog;
};