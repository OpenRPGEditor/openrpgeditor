#pragma once
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/MovePicture.hpp"
#include "Database/EventCommands/ShowPicture.hpp"

struct Dialog_MovePicture : IEventDialogController {
  Dialog_MovePicture() = delete;
  explicit Dialog_MovePicture(const std::string& name, const std::shared_ptr<MovePictureCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new MovePictureCommand());
    }
    m_number = m_command->picture;
    m_origin = static_cast<int>(m_command->origin);
    m_type = static_cast<int>(m_command->pictureLocation);
    m_xVariable = m_command->x;
    m_yVariable = m_command->y;
    m_zoomX = m_command->width;
    m_zoomY = m_command->height;
    m_opacityValue = m_command->opacity;
    m_blendMode = static_cast<int>(m_command->blendMode);
    m_duration = m_command->duration;
    m_waitForCompletion = m_command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_number;
  int m_origin;
  int m_type;
  int m_xVariable; // direct X value or indirect from global variables
  int m_yVariable; // direct Y value or indirect from global variables
  int m_zoomX;
  int m_zoomY;
  int m_opacityValue;
  int m_blendMode;
  int m_duration;
  bool m_waitForCompletion;

  bool m_isPickingY{false};
  int m_xConstant{0};
  int m_yConstant{0};

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> m_varPicker;
  std::shared_ptr<MovePictureCommand> m_command;
};