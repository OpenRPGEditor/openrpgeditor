#pragma once
#include <memory>

#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ShowPicture.hpp"

struct Dialog_ShowPicture : IEventDialogController {
  Dialog_ShowPicture() = delete;
  explicit Dialog_ShowPicture(const std::string& name, const std::shared_ptr<ShowPictureCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command = std::make_shared<ShowPictureCommand>();
    }
    m_number = m_command->number;
    m_imageName = m_command->imageName;
    m_origin = static_cast<int>(m_command->origin);
    m_type = static_cast<int>(m_command->type);
    m_xVariable = m_command->value1;
    m_yVariable = m_command->value2;
    m_zoomX = m_command->zoomX;
    m_zoomY = m_command->zoomY;
    m_opacityValue = m_command->opacityValue;
    m_blendMode = static_cast<int>(m_command->blendMode);

    m_imagePicker.emplace(ImagePicker::PickerMode::Picture, m_imageName, m_imageName);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_number;
  std::string m_imageName;
  int m_origin;
  int m_type;
  int m_xVariable; // direct X value or indirect from global variables
  int m_yVariable; // direct Y value or indirect from global variables
  int m_zoomX;
  int m_zoomY;
  int m_opacityValue;
  int m_blendMode;

  bool m_isPickingY{false};
  int m_xConstant{0};
  int m_yConstant{0};

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> m_varPicker;
  std::shared_ptr<ShowPictureCommand> m_command;
  std::optional<ImagePicker> m_imagePicker;
};