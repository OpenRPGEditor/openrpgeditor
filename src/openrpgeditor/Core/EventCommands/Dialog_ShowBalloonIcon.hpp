#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ShowBalloonIcon.hpp"

struct Dialog_ShowBalloonIcon : IEventDialogController {
  Dialog_ShowBalloonIcon() = delete;
  explicit Dialog_ShowBalloonIcon(const std::string& name, const std::shared_ptr<ShowBalloonIconCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShowBalloonIconCommand());
    }
    m_character = command->id;
    m_balloonIndex = static_cast<int>(command->index);
    m_waitCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_character;
  int m_balloonIndex;
  bool m_waitCompletion;

  bool m_confirmed{false};
  std::shared_ptr<ShowBalloonIconCommand> command;
  std::tuple<bool, bool> result;
};