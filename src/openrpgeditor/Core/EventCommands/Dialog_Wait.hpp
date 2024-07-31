#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/Wait.hpp"

struct Dialog_Wait : IEventDialogController {
  Dialog_Wait() = delete;
  explicit Dialog_Wait(const std::string& name,
                       const std::shared_ptr<WaitCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new WaitCommand());
    }
    m_waitDuration = command->duration;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_waitDuration;
  bool m_confirmed{false};
  std::shared_ptr<WaitCommand> command;
  std::tuple<bool, bool> result;
};