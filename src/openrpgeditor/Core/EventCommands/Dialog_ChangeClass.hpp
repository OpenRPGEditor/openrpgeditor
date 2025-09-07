#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeClass.hpp"

struct Dialog_ChangeClass : IEventDialogController {
  Dialog_ChangeClass() = delete;
  explicit Dialog_ChangeClass(const std::string& name, const std::shared_ptr<ChangeClassCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeClassCommand());
    }
    m_actor = m_command->actor;
    m_class = m_command->classId;
    m_saveLevel = m_command->saveLevel;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_actor;
  int m_class;
  bool m_saveLevel;

  bool m_confirmed{false};
  std::optional<ActorPicker> m_actorPicker;
  std::optional<ClassPicker> m_classPicker;
  std::shared_ptr<ChangeClassCommand> m_command;
};