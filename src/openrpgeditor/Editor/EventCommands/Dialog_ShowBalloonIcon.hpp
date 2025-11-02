#pragma once
#include "Database/EventCommands/ShowBalloonIcon.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ShowBalloonIcon : IEventDialogController {
  Dialog_ShowBalloonIcon() = delete;
  explicit Dialog_ShowBalloonIcon(const std::string& name, const std::shared_ptr<ShowBalloonIconCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ShowBalloonIconCommand());
    }
    m_character = m_command->id;
    m_balloonIndex = static_cast<int>(m_command->index);
    m_waitCompletion = m_command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_character;
  int m_balloonIndex;
  bool m_waitCompletion;

  bool m_confirmed{false};
  std::shared_ptr<ShowBalloonIconCommand> m_command;
};