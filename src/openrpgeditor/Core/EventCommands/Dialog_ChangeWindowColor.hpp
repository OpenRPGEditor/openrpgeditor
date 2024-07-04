#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeBattleback.hpp"
#include "Database/EventCommands/ChangeWindowColor.hpp"

struct Project;
struct Dialog_ChangeWindowColor : IDialogController {
  Dialog_ChangeWindowColor() = delete;
  explicit Dialog_ChangeWindowColor(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeWindowColorCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int red = 255;
  int green = 255;
  int blue = 255;

  bool m_confirmed{false};
  std::optional<ChangeWindowColorCommand> command;
  std::tuple<bool, bool> result;
};