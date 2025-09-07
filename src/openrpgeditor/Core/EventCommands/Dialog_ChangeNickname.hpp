#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Actor.hpp"

#include "Database/EventCommands/ChangeNickname.hpp"

struct Dialog_ChangeNickname : IEventDialogController {
  Dialog_ChangeNickname() = delete;
  explicit Dialog_ChangeNickname(const std::string& name, const std::shared_ptr<ChangeNicknameCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeNicknameCommand());
    }
    m_actor = m_command->actor;
    m_actorNickname = m_command->nick;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_actor = 1;
  std::string m_actorNickname;

  bool m_confirmed{false};
  std::optional<ActorPicker> m_actorPicker;
  std::shared_ptr<ChangeNicknameCommand> m_command;
};