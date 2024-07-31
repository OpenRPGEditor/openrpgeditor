#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/FadeoutBGS.hpp"

struct Dialog_FadeoutBGS : IEventDialogController {
  Dialog_FadeoutBGS() = delete;
  explicit Dialog_FadeoutBGS(const std::string& name,
                             const std::shared_ptr<FadeoutBGS>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new FadeoutBGS());
    }
    m_duration = command->duration;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_duration;
  bool m_confirmed{false};
  std::shared_ptr<FadeoutBGS> command;
  std::tuple<bool, bool> result;
};