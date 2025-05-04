#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeName.hpp"

struct Dialog_ChangeName : IEventDialogController {
  Dialog_ChangeName() = delete;
  explicit Dialog_ChangeName(const std::string& name, const std::shared_ptr<ChangeNameCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeNameCommand());
    }
    m_actor = m_command->actor;
    m_actorName = m_command->name;
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_actor;
  std::string m_actorName;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::shared_ptr<ChangeNameCommand> m_command;
};