#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/FadeoutBGM.hpp"

struct Dialog_FadeoutBGM : IEventDialogController {
  Dialog_FadeoutBGM() = delete;
  explicit Dialog_FadeoutBGM(const std::string& name, const std::shared_ptr<FadeoutBGM>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new FadeoutBGM());
    }
    m_duration = m_command->duration;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_duration;
  bool m_confirmed{false};
  std::shared_ptr<FadeoutBGM> m_command;
};