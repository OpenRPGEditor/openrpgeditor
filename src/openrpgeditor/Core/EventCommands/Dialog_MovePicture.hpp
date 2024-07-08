#pragma once
#include "IDialogController.hpp"
#include "Database/EventCommands/ShowPicture.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/MovePicture.hpp"

struct Project;
struct Dialog_MovePicture : IDialogController {
  Dialog_MovePicture() = delete;
  explicit Dialog_MovePicture(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new MovePictureCommand());
    m_number = command->picture;
    m_origin = static_cast<int>(command->origin);
    m_type = static_cast<int>(command->pictureLocation);
    m_value1 = command->x;
    m_value2 = command->y;
    m_zoomX = command->width;
    m_zoomY = command->height;
    m_opacityValue = command->opacity;
    m_blendMode = static_cast<int>(command->blendMode);
    m_duration = command->duration;
    m_waitForCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_number;
  int m_origin;
  int m_type;
  int m_value1; // direct X value or indirect from global variables
  int m_value2; // direct Y value or indirect from global variables
  int m_zoomX;
  int m_zoomY;
  int m_opacityValue;
  int m_blendMode;
  int m_duration;
  bool m_waitForCompletion;

  bool xOrY{false};
  int m_constant1{0};
  int m_constant2{0};

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<MovePictureCommand> command;
  std::tuple<bool, bool> result;
};