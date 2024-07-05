#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/RecoverAll.hpp"

struct Project;
struct Dialog_RecoverAll : IDialogController {
  Dialog_RecoverAll() = delete;
  explicit Dialog_RecoverAll(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<RecoverAllCommand>(command.value()); };
  Project* m_project = nullptr;

private:

  int m_actor = 1;
  int m_variable = 1;
  int m_Source = 0;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<VariableSwitchPicker> picker;
  std::optional<RecoverAllCommand> command;
  std::tuple<bool, bool> result;
};