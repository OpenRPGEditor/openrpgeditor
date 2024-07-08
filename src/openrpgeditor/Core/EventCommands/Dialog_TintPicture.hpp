#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/TintPicture.hpp"

struct Project;
struct Dialog_TintPicture : IEventDialogController {
  Dialog_TintPicture() = delete;
  explicit Dialog_TintPicture(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new TintPictureCommand());
    m_picture = command->picture;
    r = command->color.r;
    g = command->color.g;
    b = command->color.b;
    gray = command->color.gray;
    m_duration = command->duration;
    m_waitForCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_picture;
  int r;
  int g;
  int b;
  int gray;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<TintPictureCommand> command;
  std::tuple<bool, bool> result;
};