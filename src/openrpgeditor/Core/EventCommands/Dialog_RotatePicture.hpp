#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/RotatePicture.hpp"

struct Project;
struct Dialog_RotatePicture : IEventDialogController {
  Dialog_RotatePicture() = delete;
  explicit Dialog_RotatePicture(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new RotatePictureCommand());
    m_picture = command->picture;
    m_rotation = command->rotation;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{false};
  int m_picture;
  int m_rotation;
  std::shared_ptr<RotatePictureCommand> command;
  std::tuple<bool, bool> result;
};