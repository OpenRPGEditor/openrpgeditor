#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ChangeWindowColor.hpp"

struct Project;
struct Dialog_ChangeWindowColor : IDialogController {
  Dialog_ChangeWindowColor() = delete;
  explicit Dialog_ChangeWindowColor(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ChangeWindowColorCommand();
    red = command->r;
    green = command->g;
    blue = command->b;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int red;
  int green;
  int blue;

  bool m_confirmed{false};
ChangeWindowColorCommand* command;
  std::tuple<bool, bool> result;
};