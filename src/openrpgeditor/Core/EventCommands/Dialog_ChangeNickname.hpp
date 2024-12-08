#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Actor.hpp"

#include "Database/EventCommands/ChangeNickname.hpp"

struct Dialog_ChangeNickname : IEventDialogController {
  Dialog_ChangeNickname() = delete;
  explicit Dialog_ChangeNickname(const std::string& name, const std::shared_ptr<ChangeNicknameCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeNicknameCommand());
    }
    m_actor = command->actor;
    m_nickname = command->nick;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_actor = 1;
  std::string m_nickname = "";

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::shared_ptr<ChangeNicknameCommand> command;
  std::tuple<bool, bool> result;
};