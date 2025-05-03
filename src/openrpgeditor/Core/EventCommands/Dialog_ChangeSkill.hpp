#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeSkill.hpp"
#include "Database/Skill.hpp"

struct Dialog_ChangeSkill : IEventDialogController {
  Dialog_ChangeSkill() = delete;
  explicit Dialog_ChangeSkill(const std::string& name, const std::shared_ptr<ChangeSkillCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeSkillCommand());
    }
    m_comparison = static_cast<int>(m_command->comparison);
    m_value = m_command->value;
    m_skillOp = static_cast<int>(m_command->skillOp);
    m_skill = m_command->skill;
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_comparison;
  int m_value;
  int m_skillOp;
  int m_skill;

  int m_var{1};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<ObjectPicker<Skill>> m_skillPicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<ChangeSkillCommand> m_command;
};