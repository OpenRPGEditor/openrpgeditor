#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/FadeoutBGS.hpp"

struct Dialog_FadeoutBGS : IEventDialogController {
  Dialog_FadeoutBGS() = delete;
  explicit Dialog_FadeoutBGS(const std::string& name, const std::shared_ptr<FadeoutBGS>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new FadeoutBGS());
    }
    m_duration = m_command->duration;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_duration;
  bool m_confirmed{false};
  std::shared_ptr<FadeoutBGS> m_command;
};