#pragma once
#include <memory>

#include "Database/EventCommands/TintPicture.hpp"
#include "Editor/CommonUI/ColorTonePicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_TintPicture : IEventDialogController {
  Dialog_TintPicture() = delete;
  explicit Dialog_TintPicture(const std::string& name, const std::shared_ptr<TintPictureCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command = std::make_shared<TintPictureCommand>();
    }
    m_picture = m_command->picture;
    m_duration = m_command->duration;
    m_waitForCompletion = m_command->waitForCompletion;
    m_colorPicker.setValues(m_command->color.r, m_command->color.g, m_command->color.b, m_command->color.gray);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_picture;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<TintPictureCommand> m_command;
  std::tuple<bool, bool> result;

  ColorTonePicker m_colorPicker;
};