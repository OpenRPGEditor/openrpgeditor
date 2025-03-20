#pragma once
#include "Core/CommonUI/ImagePicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ShowPicture.hpp"

struct Dialog_ShowPicture : IEventDialogController {
  Dialog_ShowPicture() = delete;
  explicit Dialog_ShowPicture(const std::string& name, const std::shared_ptr<ShowPictureCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShowPictureCommand());
    }
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

    m_imagePicker.emplace(ImagePicker::PickerMode::Picture, m_imageName, m_imageName);
    m_imagePicker->setOpen(true);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

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

  bool xOrY{false};
  int m_constant1{0};
  int m_constant2{0};

  bool m_confirmed{false};
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<ShowPictureCommand> command;
  std::optional<ImagePicker> m_imagePicker;
  std::tuple<bool, bool> result;
};