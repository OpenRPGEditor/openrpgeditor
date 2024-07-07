#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/FlashScreen.hpp"

struct Project;
struct Dialog_FlashScreen : IDialogController {
  Dialog_FlashScreen() = delete;
  explicit Dialog_FlashScreen(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new FlashScreenCommand());
    r = command->color.r;
    g = command->color.g;
    b = command->color.b;
    m_intensity = command->color.intensity;
    m_duration = command->duration;
    m_waitForCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int r;
  int g;
  int b;
  int m_intensity;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<FlashScreenCommand> command;
  std::tuple<bool, bool> result;
};