#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangePartyMember.hpp"
#include "Database/States.hpp"

struct Dialog_ChangePartyMember : IEventDialogController {
  Dialog_ChangePartyMember() = delete;
  explicit Dialog_ChangePartyMember(const std::string& name, const std::shared_ptr<ChangePartyMemberCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangePartyMemberCommand());
    }
    m_actor = command->member;
    m_operation = static_cast<int>(command->operation);
    m_isInit = command->initialize;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_actor;
  int m_operation;
  bool m_isInit;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::shared_ptr<ChangePartyMemberCommand> command;
  std::tuple<bool, bool> result;
};