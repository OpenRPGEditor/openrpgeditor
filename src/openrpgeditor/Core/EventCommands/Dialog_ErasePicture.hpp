#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ErasePicture.hpp"

struct Project;
struct Dialog_ErasePicture : IDialogController {
  Dialog_ErasePicture() = delete;
  explicit Dialog_ErasePicture(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new ErasePictureCommand());
    m_picture = command->picture;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  bool m_confirmed{false};
  int m_picture;
  std::shared_ptr<ErasePictureCommand> command;
  std::tuple<bool, bool> result;
};