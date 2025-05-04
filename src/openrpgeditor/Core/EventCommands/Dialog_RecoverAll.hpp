#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/RecoverAll.hpp"

struct Dialog_RecoverAll : IEventDialogController {
  Dialog_RecoverAll() = delete;
  explicit Dialog_RecoverAll(const std::string& name, const std::shared_ptr<RecoverAllCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new RecoverAllCommand());
    }
    m_actor = m_command->value;
    m_variable = m_command->value;
    m_source = static_cast<int>(m_command->comparison);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_actor;
  int m_variable;
  int m_source;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<RecoverAllCommand> m_command;
};