#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/EventCommands/ChangeSkill.hpp"
#include "Database/Skills.hpp"
#include "Database/Actors.hpp"

struct Project;
struct Dialog_ChangeSkill : IDialogController {
  Dialog_ChangeSkill() = delete;
  explicit Dialog_ChangeSkill(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.reset(new ChangeSkillCommand());
    m_comparison = static_cast<int>(command->comparison);
    m_value = command->value;
    m_skillOp = static_cast<int>(command->skillOp);
    m_skill = command->skill;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

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