#pragma once
#include "Core/CommonUI/ColorTonePicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/TintPicture.hpp"

struct Dialog_TintPicture : IEventDialogController {
  Dialog_TintPicture() = delete;
  explicit Dialog_TintPicture(const std::string& name, const std::shared_ptr<TintPictureCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new TintPictureCommand());
    }
    m_picture = m_command->picture;
    r = m_command->color.r;
    g = m_command->color.g;
    b = m_command->color.b;
    gray = m_command->color.gray;
    m_duration = m_command->duration;
    m_waitForCompletion = m_command->waitForCompletion;
    m_colorPicker.setValues(r, g, b, gray);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_picture;
  int r;
  int g;
  int b;
  int gray;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<TintPictureCommand> m_command;
  std::tuple<bool, bool> result;

  ColorTonePicker m_colorPicker;
};