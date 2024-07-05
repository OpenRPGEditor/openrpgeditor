#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/States.hpp"
#include "Database/EventCommands/ChangePartyMember.hpp"

struct Project;
struct Dialog_ChangePartyMember : IDialogController {
  Dialog_ChangePartyMember() = delete;
  explicit Dialog_ChangePartyMember(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangePartyMemberCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int m_actor = 1;
  int m_operation = 0;
  bool m_isInit = false;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<ChangePartyMemberCommand> command;
  std::tuple<bool, bool> result;
};