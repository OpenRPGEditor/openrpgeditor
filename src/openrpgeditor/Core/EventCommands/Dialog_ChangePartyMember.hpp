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
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangePartyMemberCommand());
    }
    m_actor = m_command->member;
    m_operation = static_cast<int>(m_command->operation);
    m_isInit = m_command->initialize;
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_actor;
  int m_operation;
  bool m_isInit;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::shared_ptr<ChangePartyMemberCommand> m_command;
};