#pragma once
#include "Database/EventCommands/ResumeBGM.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ReplayBGM : IEventDialogController {
  Dialog_ReplayBGM() = delete;
  explicit Dialog_ReplayBGM(const std::string& name, const std::shared_ptr<ResumeBGMCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ResumeBGMCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<ResumeBGMCommand> command;
  std::tuple<bool, bool> result;
};