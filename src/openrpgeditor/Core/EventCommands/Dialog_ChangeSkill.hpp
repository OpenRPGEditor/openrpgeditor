#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeSkill.hpp"
#include "Database/Skills.hpp"

struct Dialog_ChangeSkill : IEventDialogController {
  Dialog_ChangeSkill() = delete;
  explicit Dialog_ChangeSkill(const std::string& name, const std::shared_ptr<ChangeSkillCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeSkillCommand());
    }
    m_comparison = static_cast<int>(command->comparison);
    m_value = command->value;
    m_skillOp = static_cast<int>(command->skillOp);
    m_skill = command->skill;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_comparison;
  int m_value;
  int m_skillOp;
  int m_skill;

  int m_var{1};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<ObjectPicker<Skill>> skill_picker;
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<ChangeSkillCommand> command;
  std::tuple<bool, bool> result;
};