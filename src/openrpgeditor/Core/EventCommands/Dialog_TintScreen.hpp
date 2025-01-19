#pragma once
#include "Core/CommonUI/ColorTonePicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/TintScreen.hpp"
#include <algorithm>

struct Dialog_TintScreen : IEventDialogController {
  Dialog_TintScreen() = delete;
  explicit Dialog_TintScreen(const std::string& name, const std::shared_ptr<TintScreenCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new TintScreenCommand());
    }
    r = command->color.r;
    g = command->color.g;
    b = command->color.b;
    gray = command->color.gray;
    m_duration = command->duration;
    m_waitForCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int r;
  int g;
  int b;
  int gray;
  int m_duration;
  int m_currentTemplate{-1};
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<TintScreenCommand> command;
  std::tuple<bool, bool> result;

  ColorTonePicker color_picker;
};