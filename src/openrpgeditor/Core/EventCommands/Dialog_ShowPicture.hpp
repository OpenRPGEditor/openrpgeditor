#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ShowPicture.hpp"

struct Project;
struct Dialog_ShowPicture : IEventDialogController {
  Dialog_ShowPicture() = delete;
  explicit Dialog_ShowPicture(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new ShowPictureCommand());
    m_number = command->number;
    m_imageName = command->imageName;
    m_origin = static_cast<int>(command->origin);
    m_type = static_cast<int>(command->type);
    m_value1 = command->value1;
    m_value2 = command->value2;
    m_zoomX = command->zoomX;
    m_zoomY = command->zoomY;
    m_opacityValue = command->opacityValue;
    m_blendMode = static_cast<int>(command->blendMode);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_number;
  std::string m_imageName;
  int m_origin;
  int m_type;
  int m_value1; // direct X value or indirect from global variables
  int m_value2; // direct Y value or indirect from global variables
  int m_zoomX;
  int m_zoomY;
  int m_opacityValue;
  int m_blendMode;

  bool m_confirmed{false};
  std::shared_ptr<ShowPictureCommand> command;
  std::tuple<bool, bool> result;
};