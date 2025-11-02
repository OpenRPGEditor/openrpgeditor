#pragma once
#include "Database/EventCommands/TintScreen.hpp"
#include "Editor/CommonUI/ColorTonePicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include <algorithm>
#include <memory>

struct Dialog_TintScreen : IEventDialogController {
  Dialog_TintScreen() = delete;
  explicit Dialog_TintScreen(const std::string& name, const std::shared_ptr<TintScreenCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command = std::make_shared<TintScreenCommand>();
    }
    m_duration = m_command->duration;
    m_waitForCompletion = m_command->waitForCompletion;

    m_colorPicker.setValues(m_command->color.r, m_command->color.g, m_command->color.b, m_command->color.gray);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_duration;
  int m_currentTemplate{-1};
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<TintScreenCommand> m_command;

  ColorTonePicker m_colorPicker;
};