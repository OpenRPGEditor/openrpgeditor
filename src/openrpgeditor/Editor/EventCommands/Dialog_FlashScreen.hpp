#pragma once
#include "Database/EventCommands/FlashScreen.hpp"
#include "Editor/CommonUI/ColorFlashPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_FlashScreen : IEventDialogController {
  Dialog_FlashScreen() = delete;
  explicit Dialog_FlashScreen(const std::string& name, const std::shared_ptr<FlashScreenCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new FlashScreenCommand());
    }
    r = m_command->color.r;
    g = m_command->color.g;
    b = m_command->color.b;
    m_intensity = m_command->color.intensity;
    m_duration = m_command->duration;
    m_waitForCompletion = m_command->waitForCompletion;

    m_colorPicker.setValues(r, g, b, m_intensity);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int r;
  int g;
  int b;
  int m_intensity;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<FlashScreenCommand> m_command;
  std::tuple<bool, bool> result;

  ColorFlashPicker m_colorPicker;
};