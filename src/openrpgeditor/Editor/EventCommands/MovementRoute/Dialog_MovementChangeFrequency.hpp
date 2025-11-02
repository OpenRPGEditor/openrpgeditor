#pragma once
#include "Database/EventCommands/MovementRoute/Frequency.hpp"
#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct MainWindow;
struct Dialog_MovementChangeFrequency : IEventDialogController {
  Dialog_MovementChangeFrequency() = delete;
  explicit Dialog_MovementChangeFrequency(const std::string& name, const std::shared_ptr<MovementFrequencyCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (m_command == nullptr) {

      m_command.reset(new MovementFrequencyCommand());
    }
    m_frequency = m_command->frequency;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_frequency;
  std::shared_ptr<MovementFrequencyCommand> m_command;
};